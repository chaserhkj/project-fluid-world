#ifndef _INTERFACE_H_
#define _INTERFACE_H_

class Project
{
public:
    Project() {}
    virtual ~Project() {}
    virtual void initialize() = 0;
    virtual void run() = 0;
    virtual void spotstainrun() = 0;
    virtual double getT() = 0;
};

#endif /* _INTERFACE_H_ */
