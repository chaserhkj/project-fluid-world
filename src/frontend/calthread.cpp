#include "calthread.h"
#include "../2D/cylinder.h"
#include <QtCore>

CalThread::CalThread(QObject * parent) : QThread(parent), stopCalled(false)
{
    pro = new cylinderProject(-50, 300, 50, -50, 0.2, 0.2, 0.2, 0.2, 20);
}

CalThread::~CalThread()
{
    delete pro;
}

void CalThread::start()
{
    stopCalled = false;
    this->QThread::start();
}

void CalThread::run()
{
    int j=0;
    while(j<40)
    {
        int i;
        qDebug()<<j;
        for (i = 0; i < 1; ++i) {
            if (stopCalled)
            {
                emit calculateInterrupted();
                return;
            }
            pro->run();
        }
        for (i = 0; i < 1000; ++i)
            pro->spotstainrun();            
        spotStainTable table;
        QList<QPointF> line;
        DataVariant * d = pro->getData(Project::NumberType);
        int n = d->getNumber();
        delete d;
        for (i = 0; i < n; ++i)
        {
            line.clear();
            d = pro->getData(Project::SpotType, i);
            do
                line.append(QPointF(d->getX(),d->getY()));
            while(d->next());
            delete d;
            table.append(line);
        }
        this->putData(table);
        emit dataGenerated();
        ++j;
    }
    emit calculateFinished();
}

void CalThread::stop()
{
    stopCalled = true;
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
