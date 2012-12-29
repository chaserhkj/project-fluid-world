#include <math.h> //TODO:Change to cmath?
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
    return;
}


void cylinderProject::initialize()
{
    int i,j;
    for (i = downboundary; i <= upboundary; i++){
        for (j = leftboundary; j <= rightboundary; j++){
            coordination->access(j,i).setXi(j * deltaxi);
            coordination->access(j,i).seteta(j * deltaeta);
            coordination->access(j,i).calculateXY();
        }
    }
    //TODO: Boundary conditions
    return;
}

void cylinderProject::run(/*double psi1, double psi2, double psi3, double psi4 Do one recursive calculation, psi1=psi(i+1,j), psi2=psi(i-1,j), psi3=psi(i,j+1), psi4=psi(i,j-1) */)
{
    double hxi, heta; /* lame coefficient */
    double g1, g2, g3, g4;
    double b0, b1, b2, b3, b4;
    double x, y;
    //double omega = 1.618; /* relaxation coefficient */
    double vxi, veta;
    double uxi, ueta;
    double lambdaxi, lambdaeta;
    double c1, c2, c3, c4, c5, c6, c7, c8, c9, c10;
    int i,j;
    for (i = downboundary + 1; i < upboundary; i++){
        for (j = leftboundary + 1; j < rightboundary; j++){
            //recursive calcultion of Psi
            //TODO: Boundary conditions
            x = coordination->access(j, i).getX();
            y = coordination->access(j, i).getY();
            hxi = sqrt(1 + (1 + x * x - y * y) / ((x * x + y * y) * (x * x + y * y))) / 2;
            heta = hxi;
            g1 = 1 / (hxi * hxi), g2 = 1 / (heta * heta), g3 = 0, g4 = 0;
            b0 = 2 * g1 / (deltaxi * deltaxi) + 2 * g2 / (deltaeta * deltaeta);
            b1 = g1 / (deltaxi * deltaxi) + g3 / (2 * deltaxi);
            b2 = g1 / (deltaxi * deltaxi) - g3 / (2 * deltaxi);
            b3 = g2 / (deltaeta * deltaeta) + g4 / (2 * deltaeta);
            b4 = g2 / (deltaeta * deltaeta) - g4 / (2 * deltaeta);
            //newpsi = (1 - omega) * psi + omega * (b1 * psi1 + b2 * psi2 + b3 * psi3 + b4 * psi4 + zeta) / b0;
            //FIXME:More to do here...

            //recursive calculation of Zeta
            vxi = (coordination->access(j, i + 1).getPsi() - coordination->access(j, i - 1).getPsi()) / (2 * heta * deltaeta);
            veta = -(coordination->access(j + 1, i).getPsi() - coordination->access(j + 1, i).getPsi()) / (2 * hxi * deltaxi);
            uxi = vxi / hxi;
            ueta = veta / heta;
            if (Re < 1000) {
                lambdaxi = 1 - 1 / exp(fabs(uxi));
                lambdaeta = 1 - 1 / exp(fabs(ueta));
            } else {
                lambdaxi = 1;
                lambdaeta = 1;
            }
            c1 = -2 / deltat - lambdaxi * fabs(uxi) / (2 * deltaxi) - lambdaeta * fabs(ueta) / (2 * deltaeta) - 4 / (hxi * hxi * deltaxi * deltaxi * Re) - 4 / (heta * heta * deltaeta * deltaeta * Re);
            c2 = -2 / deltat + lambdaxi * fabs(uxi) / (2 * deltaxi) + lambdaeta * fabs(ueta) / (2 * deltaeta) + 4 / (hxi * hxi * deltaxi * deltaxi * Re) + 4 / (heta * heta * deltaeta * deltaeta * Re);
            c3 = - (ueta - lambdaeta * fabs(ueta)) / (12 * deltaeta);
            c4 = (2 * ueta - lambdaeta * fabs(ueta)) / (3 * deltaeta) - 2 / (heta * heta * deltaeta * deltaeta * Re);
            c5 = -(2 * ueta + lambdaeta * fabs(ueta)) / (3 * deltaeta) - 2 / (heta * heta * deltaeta * deltaeta * Re);
            c6 = (ueta + lambdaeta * fabs(ueta)) / (12 * deltaeta);
            c7 = - (uxi - lambdaxi * fabs(uxi)) / (12 * deltaxi);
            c8 = (2 * uxi - lambdaxi * fabs(uxi)) / (3 * deltaxi) - 2 / (hxi * hxi * deltaxi * deltaxi * Re);
            c9 = -(2 * uxi + lambdaxi * fabs(uxi)) / (3 * deltaxi) - 2 / (hxi * hxi * deltaxi * deltaxi * Re);
            c10 = (uxi + lambdaxi * fabs(uxi)) / (12 * deltaxi);
        }
    }


    t += deltat;
}
