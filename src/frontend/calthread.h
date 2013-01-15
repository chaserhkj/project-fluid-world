#ifndef _CALTHREAD_H_
#define _CALTHREAD_H_

#include <QThread>
class cylinderProject;
class Project;

class CalThread : public QThread
{
    Q_OBJECT;
public:
    CalThread(QObject * parent = 0);
    virtual ~CalThread();
    void run();
    Project * getProject();
//    void setProject(cylinderProject * pro);
//    cylinderProject * project();
private:
    cylinderProject * pro;
signals:
    void dataGenerated();
};


#endif /* _CALTHREAD_H_ */
