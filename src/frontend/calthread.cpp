#include "calthread.h"
#include "../2D/cylinder.h"
#include <QtCore>

CalThread::CalThread(QObject * parent) : QThread(parent), stopCalled(false)
{
    pro = new cylinderProject;
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
    while(!stopCalled)
    {
        int i;
        for (i = 0; i < 1; ++i) {
            if (stopCalled)
                return;
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
    }
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
