#ifndef _CALTHREAD_H_
#define _CALTHREAD_H_

#include <QThread>
#include "backend.h"

class QObject;

class CalThread : public QThread, public pendulum
{
Q_OBJECT   
public:
    CalThread(double t = 0, double v = 0, double l = 1, double s = 0.1, QObject *parent = 0): \
	QThread(parent),pendulum(t,v,l,s),quitCalled(false){}
signals:
    void valueChanged(double position, double velocity);
public slots:
    void quit();
    void start(Priority p = InheritPriority);
protected:
    virtual void run();
private:
    bool quitCalled;
    static const int freshTime;
};

#endif /* _CALTHREAD_H_ */
