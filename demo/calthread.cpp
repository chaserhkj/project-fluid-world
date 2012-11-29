#include "calthread.h"

const int freshTime = 10;

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
    int i, times = freshTime/(step*1000);
    while(!quitCalled)
    {
	for (i = 0; i < times; ++i)
	{
	    this->pendulum::run();
	}
	emit valueChanged(this->getAngle(),this->getVelocity());
	QThread::msleep(freshTime);
    }
}
