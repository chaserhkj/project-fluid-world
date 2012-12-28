#include <math.h>
#include "cylinder.h"

void cylinderNode::calculateXY()
{
    double delta = xi * xi - eta * eta - 1;
    double theta;
    if (delta > 0){
        theta = atan(2 * xi * eta / delta);
    } else {
        if (delta == 0){
            theta = M_PI_2; /* pi/2 */
        } else {
            theta = M_PI + atan(2 * xi * eta / delta);
        }
    }
    double mid = sqrt(sqrt(delta * delta + 4 * xi * xi * eta * eta));
    x = xi + fabs(xi) * mid * fabs(cos(theta / 2));
    y = eta + fabs(eta) * mid * fabs(sin(theta / 2));
}

void cylinderNode::run(double psi1, double psi2, double psi3, double psi4/* Do one recursive calculation, psi1=psi(i+1,j), psi2=psi(i-1,j), psi3=psi(i,j+1), psi4=psi(i,j-1) */)
{
    //recursive calcultion of Psi
    double hxi, heta; /* lame coefficient */
    hxi = sqrt(1 + (1 + x * x - y * y) / ((x * x + y * y) * (x * x + y * y))) / 2;
    heta = hxi;
    double g1 = 1 / (hxi * hxi), g2 = 1 / (heta * heta), g3 = 0, g4 = 0;
    double b0, b1, b2, b3, b4;
    b0 = 2 * g1 / (deltaxi * deltaxi) + 2 * g2 / (deltaeta * deltaeta);
    b1 = g1 / (deltaxi * deltaxi) + g3 / (2 * deltaxi);
    b2 = g1 / (deltaxi * deltaxi) - g3 / (2 * deltaxi);
    b3 = g2 / (deltaeta * deltaeta) + g4 / (2 * deltaeta);
    b4 = g2 / (deltaeta * deltaeta) - g4 / (2 * deltaeta);
    double omega = 1.618; /* relaxation coefficient */
    newpsi = (1 - omega) * psi + omega * (b1 * psi1 + b2 * psi2 + b3 * psi3 + b4 * psi4 + zeta) / b0;

    //recursive calculation of Zeta
    
}
