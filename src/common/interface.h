#ifndef _INTERFACE_H_
#define _INTERFACE_H_

class DataVariant;

class Project
{
public:
    Project() {}
    virtual ~Project() {}
    enum DataType{NoneType, TimeType, PsiType, SpotType};
    virtual void initialize() = 0;
    virtual void run()= 0;
    virtual double getData(DataType data, ...) = 0;
};

class DataVariant
{
private:
    Project::DataType type;
public:
    DataVariant(Project::DataType t = Project::NoneType);
    virtual double getX() = 0;
    virtual double getY() = 0;
    virtual double getZ() = 0;
    virtual double getPsi() = 0;
    virtual double getTime() = 0;
    virtual bool next() = 0;
};


#endif /* _INTERFACE_H_ */
