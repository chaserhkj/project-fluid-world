#include "calthread.h"

const int CalThread::freshTime = 10;

void CalThread::quit()
{
    quitCalled = true;
    paused = false;
}

void CalThread::start(Priority p)
{
    quitCalled = false;
    paused = defaultPaused;
    emit positionChanged(this->getAngle());
    emit velocityChanged(this->getVelocity());
    emit timeChanged(this->getTime());
    this->QThread::start(p);
}

void CalThread::togglePause()
{
    if(paused)
	paused = false;
    else
	paused = true;
}


void CalThread::run()
{
    int i, times = freshTime/(step*1000);
    while(!quitCalled)
    {
	while(paused)
	{
	    QThread::msleep(freshTime);
	}
	for (i = 0; i < times; ++i)
	{
	    this->pendulum::run();
	}
	emit positionChanged(this->getAngle());
	emit velocityChanged(this->getVelocity());
	emit timeChanged(this->getTime());
	QThread::msleep(freshTime);
    }
}
