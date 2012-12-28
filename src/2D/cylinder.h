#ifndef _cylinder
#define _cylinder

#include "commontypes.h"

class cylinderNode : public Node {};

class cylinderProject{
private:
    double t; /* Physical time */
protected:
    static const double deltaxi; /* delta xi and delta eta */
    static const double deltaeta; /* delta xi and delta eta */
    static const double deltat; /* time step */
public:
    cylinderProject(double dxi = 0.1, double deta = 0.1, double dt = 0.1):

#endif /* _cylinder */

/* radius = 1 */
