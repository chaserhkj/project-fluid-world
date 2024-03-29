#ifndef _INTERFACE_H_
#define _INTERFACE_H_

class DataVariant;

class Project
{
public:
    Project() {
    }
    virtual ~Project() {
    }
    enum DataType {NoneType, TimeType, PsiType, SpotType,
                   NumberType     /* number of spotstain sources */
                  };
    virtual void initialize() = 0;
    virtual void run() = 0;
    virtual void spotstainrun() = 0;
    virtual void setDensity(double dens) = 0;
    virtual bool dumptofile(const char * location) = 0;
    virtual DataVariant * getData(DataType type, ...) = 0;
};

class DataVariant
{
protected:
    Project::DataType type;
public:
    DataVariant() {
    }
    virtual double getX() = 0;
    virtual double getY() = 0;
    virtual double getPsi() = 0;
    virtual double getTime() = 0;
    virtual int getNumber() = 0;
    virtual bool next() = 0;         //If successfully get next spotstain, then return true, else return false
};


#endif /* _INTERFACE_H_ */
