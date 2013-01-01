#ifndef _INTERFACE_H_
#define _INTERFACE_H_

class Project
{
public:
    Project();
    virtual ~Project();
    virtual initialize() = 0;
    virtual run() = 0;
    virtual spotstainrun() = 0;
    virtual getT() = 0;
};

#endif /* _INTERFACE_H_ */
