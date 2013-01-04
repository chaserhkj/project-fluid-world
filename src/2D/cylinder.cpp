#include <cmath>
#include "cylinder.h"

using std::abs;

inline double unitfy(double v)
{
    if (v == 0) {
        return 0;
    } else {
        return v > 0 ? 1 : -1;
    }
}

void cylinderTransformer::calculateXY()
{
    double delta = xi * xi - eta * eta - 1;
    double theta;

    if (delta > 0) {
        theta = atan(2 * xi * eta / delta);
    } else {
        if (delta == 0) {
            theta = M_PI_2;   /* pi/2 */
        } else {
            theta = M_PI + atan(2 * xi * eta / delta);
        }
    }

    double mid = sqrt(sqrt(delta * delta + 4 * xi * xi * eta * eta));
    x = xi + unitfy(xi) * mid * abs(cos(theta / 2));
    y = eta + unitfy(eta) * mid * abs(sin(theta / 2));
    return;
}
