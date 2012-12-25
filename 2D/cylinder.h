#ifndef _cylinder
#define _cylinder

#include "commontypes.h"

class cylinderNode : public Node {
private:
    static const double deltaxi = 0.01; /* delta xi and delta eta */
    static const double deltaeta = 0.01; /* delta xi and delta eta */
};

#endif /* _cylinder */

/* radius = 1 */
