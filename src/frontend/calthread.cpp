#include "calthread.h"
#include "../2D/cylinder.h"

CalThread::CalThread(QObject * parent) : QThread(parent)
{
    pro = new cylinderProject;
}

CalThread::~CalThread()
{
    delete pro;
}

Project * CalThread::getProject()
{
    return pro;
}

void CalThread::run()
{
//    while(1)
    {
        int i;
        for (i = 0; i < 100; ++i)
            pro->run();
        for (i = 0; i < 1000; ++i)
            pro->spotstainrun();            
        emit dataGenerated();
        qDebug("Data Generated!");
    }
}
