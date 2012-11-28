#include "calthread.h"

void CalThread::quit()
{
    quitCalled = true;
}

void CalThread::start(Priority p)
{
    quitCalled = false;
    this->QThread::start(p);
}

void CalThread::run()
{
    while(!quitCalled)
    {
	this->pendulum::run();
	emit valueChanged(this->getAngle(),this->getVelocity());
	QThread::msleep(step*1000);
    }
}
