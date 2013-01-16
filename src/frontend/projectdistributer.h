#ifndef _PROJECTDISTRIBUTER_H_
#define _PROJECTDISTRIBUTER_H_

#include <QString>
#include <QObject>
class Project;

class ProjectDistributer : public QObject
{
public:
    ProjectDistributer(QObject * parent = 0) : QObject(parent) {
    }
    virtual ~ProjectDistributer() {
    }
    enum ProjectType
    { NoneType = -1 };
    static const int projectCount;
    virtual void setType(ProjectType type) = 0;
    virtual ProjectType type() = 0;
    virtual Project * construct(Project * pro, ...) = 0;
    virtual void destruct(Project * pro) = 0;
    virtual QString getTypeName() = 0;
    virtual static QString toTypeName(ProjectType type) = 0;
};

#endif /* _PROJECTDISTRIBUTER_H_ */
