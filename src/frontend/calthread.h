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
    CalThread(QObject * parent = 0);
    virtual ~CalThread();
    void start();
    void run();
    void stop();
    spotStainTable getData();
    bool queueIsEmpty();
    //    void setProject(cylinderProject * pro);
    //    cylinderProject * project();
private:
    cylinderProject * pro;
    bool stopCalled;
    QQueue<spotStainTable> data;
    QReadWriteLock lock;
    void putData(const spotStainTable & table);
signals:
    void dataGenerated();
    void calculateFinished();
    void calculateInterrupted();
};


#endif /* _CALTHREAD_H_ */
