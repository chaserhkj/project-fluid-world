#ifndef _INTERFACE_H_
#define _INTERFACE_H_

class Project
{
public:
    Project() {}
    virtual ~Project() {}
    virtual void initialize() = 0;
    virtual void run(const char * type = 0) = 0;
    virtual double getData(const char * type, const int * coordinate = 0) = 0;
};

#endif /* _INTERFACE_H_ */
