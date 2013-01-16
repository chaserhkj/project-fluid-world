#include "calthread.h"
#include "../2D/cylinder.h"
#include <QtCore>

CalThread::CalThread(QObject * parent,
                     int l,
                     int r,
                     int u,
                     int d,
                     double dens,
                     double dxi,
                     double deta,
                     double dt,
                     double rey,
                     int total,
                     int single,
                     int spot) : QThread(parent), stopCalled(false), isPaused(
                             false),
    totalCycleCount(total), singleCycleCount(single), spotCycleCount(spot)
{
    pro = new cylinderProject(l, r, u, d, dens, dxi, deta, dt, rey);
}

CalThread::~CalThread()
{
    delete pro;
}

void CalThread::start()
{
    stopCalled = false;
    isPaused = false;
    emit calculateStarted();
    this->QThread::start();
}

void CalThread::run()
{
    int j;

    for (j = 0; j < totalCycleCount; ++j) {
        int i;

        for (i = 0; i < singleCycleCount; ++i) {
            if (stopCalled) {
                emit calculateInterrupted();
                return;
            }

            while (isPaused)
                if (stopCalled) {
                    emit calculateInterrupted();
                    return;
                }

            pro->run();
        }

        for (i = 0; i < spotCycleCount; ++i)
            pro->spotstainrun();

        spotStainTable table;
        QList<QPointF> line;
        DataVariant * d = pro->getData(Project::NumberType);
        int n = d->getNumber();
        delete d;

        for (i = 0; i < n; ++i) {
            line.clear();
            d = pro->getData(Project::SpotType, i);

            do
                line.append(QPointF(d->getX(), d->getY()));

            while (d->next());

            delete d;
            table.append(line);
        }

        this->putData(table);
        emit dataGenerated();
    }

    emit calculateFinished();
}

void CalThread::stop()
{
    stopCalled = true;
}

void CalThread::togglePaused()
{
    isPaused = !isPaused;
}

void CalThread::putData(const spotStainTable & table)
{
    QWriteLocker locker(&lock);
    data.enqueue(table);
}

spotStainTable CalThread::getData()
{
    QWriteLocker locker(&lock);
    spotStainTable table = data.dequeue();
    return table;
}

bool CalThread::queueIsEmpty()
{
    QReadLocker locker(&lock);
    bool value = data.isEmpty();
    return value;
}
