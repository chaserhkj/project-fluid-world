#ifndef _INTERFACE_H_
#define _INTERFACE_H_

enum DATA{TIME, PSI, SPOT};

class FWDataVarient;

class Project
{
public:
    Project() {}
    virtual ~Project() {}
    virtual void initialize() = 0;
    virtual void run(const char * type = 0) = 0;
    virtual double getData(DATA data) = 0;
    virtual FWDataVarient getData(DATA data, double xip, double etap) = 0;
    virtual FWDataVarient getData(DATA data, int n) = 0;

    virtual double getX(FWDataVarient& data) = 0;
    virtual double getY(FWDataVarient& data) = 0;
    virtual double getPsi(FWDataVarient& data) = 0;
    virtual bool next(FWDataVarient& data) = 0;
};

#endif /* _INTERFACE_H_ */
