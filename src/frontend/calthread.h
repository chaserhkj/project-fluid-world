#ifndef _CALTHREAD_H_
#define _CALTHREAD_H_

#include <QThread>
#include <QList>
#include <QPointF>
#include <QQueue>
#include <QReadWriteLock>
class cylinderProject;
class Project;

typedef QList<QList<QPointF> > spotStainTable;

class CalThread : public QThread
{
    Q_OBJECT;
public:
    CalThread(QObject * parent,
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
              int spot);
    virtual ~CalThread();
    void run();
    spotStainTable getData();
    bool queueIsEmpty();
public slots:
    void start();
    void stop();
    void togglePaused();
private:
    cylinderProject * pro;
    bool stopCalled;
    bool isPaused;
    int totalCycleCount;
    int singleCycleCount;
    int spotCycleCount;
    QQueue<spotStainTable> data;
    QReadWriteLock lock;
    void putData(const spotStainTable & table);
signals:
    void dataGenerated();
    void calculateStarted();
    void calculateFinished();
    void calculateInterrupted();
};


#endif /* _CALTHREAD_H_ */
