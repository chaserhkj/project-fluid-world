#ifndef _CALTHREAD_H_
#define _CALTHREAD_H_

#include <QThread>
#include "backend.h"

class QObject;

class CalThread : public QThread, public pendulum
{

    Q_OBJECT;

public:
    CalThread(double t = 0,
              double v = 0,
              double l = 1,
              double s = 0.1,
              bool p = false,
              QObject * parent = 0) :
        QThread(parent), pendulum(t, v, l, s), quitCalled(false), paused(p),
        defaultPaused(p) {
        QObject::connect(this, SIGNAL(statusChanged()), this,
                         SLOT(emitChangeSignals()));
    }
signals:
    void positionChanged(double value);
    void velocityChanged(double value);
    void timeChanged(double value);
    void statusChanged();
public slots:
    void quit();
    void togglePause();
    void start(Priority p = InheritPriority);
protected:
    virtual void run();
private:
    bool quitCalled;
    bool paused;
    bool defaultPaused;
    static const int freshTime;
private slots:
    void emitChangeSignals();
};

#endif /* _CALTHREAD_H_ */
