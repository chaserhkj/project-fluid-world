#include <cmath> 
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
    x = xi + xi * mid * abs(cos(theta / 2) / xi);
    y = eta + eta * mid * abs(sin(theta / 2) / eta);
    return;
}


void cylinderProject::initialize()
{
    int i,j;
    double g1, g2, g3, g4;
    double x, y;
    cylinderNode& node;
    for (i = downboundary; i <= upboundary; i++){
        for (j = leftboundary; j <= rightboundary; j++){
            node = coordination->access(j,i);
            node.xi = j * deltaxi;
            node.eta = i * deltaeta;
            node.calculateXY();
            x = node.x;
            y = node.y;

            node.hxi = sqrt(1 + (1 + x * x - y * y) / ((x * x + y * y) * (x * x + y * y))) / 2;
            node.heta = node.hxi;

            g1 = 1 / (node.hxi * node.hxi);
            g2 = 1 / (node.heta * node.heta);
            g3 = 0;
            g4 = 0;
            node.b0 = 2 * g1 / (deltaxi * deltaxi) + 2 * g2 / (deltaeta * deltaeta);
            node.b1 = g1 / (deltaxi * deltaxi) + g3 / (2 * deltaxi);
            node.b2 = g1 / (deltaxi * deltaxi) - g3 / (2 * deltaxi);
            node.b3 = g2 / (deltaeta * deltaeta) + g4 / (2 * deltaeta);
            node.b4 = g2 / (deltaeta * deltaeta) - g4 / (2 * deltaeta);

            /* Initial conditions */
            node.psi = y * (1 - 1 / (x * x + y * y));
            node.zeta = 0;
        }
    }
    //Boundary conditions: left boundary
    for (i = downboundary; i <= upboundary; i++){
        coordination->access(leftboundary,i).psi = coordination->access(leftboundary,i).y;
        /* zeta is already 0 */
    }
    //Boundary conditions: right boundary TODO:differential restrict
    for (i = downboundary; i <= upboundary; i++){
        coordination->access(rightboundary,i).psi = coordination->access(rightboundary,i).y;
        /* zeta is already 0 */
    }
    //Boundary conditions: up and down boundary
    /* psi is already given */
    /* zeta is already 0 */

    //Boundary conditions: cylinder
    for (j = -1 / deltaxi; j <= 1 /deltaxi; j++){
        coordination->access(j,0).psi = 0;
        /* zeta is already 0 */ 
    }
    
    return;
}

void cylinderProject::run()
{
    double vxi, veta;
    double uxi, ueta;
    double lambdaxi, lambdaeta;
    int i,j;

    /* Calculating boundary conditions of zeta */

    for (i = downboundary + 1; i < upboundary; i++){
        for (j = leftboundary + 1; j < rightboundary; j++){
            //recursive calcultion of Psi
            //TODO: Boundary conditions
            //newpsi = (1 - omega) * psi + omega * (b1 * psi1 + b2 * psi2 + b3 * psi3 + b4 * psi4 + zeta) / b0;
            //FIXME:More to do here...

            //recursive calculation of Zeta
            vxi = (coordination->access(j, i + 1).getPsi() - coordination->access(j, i - 1).getPsi()) / (2 * heta * deltaeta);
            veta = -(coordination->access(j + 1, i).getPsi() - coordination->access(j + 1, i).getPsi()) / (2 * hxi * deltaxi);
            uxi = vxi / hxi;
            ueta = veta / heta;
            if (Re < 1000) {
                lambdaxi = 1 - 1 / exp(abs(uxi));
                lambdaeta = 1 - 1 / exp(abs(ueta));
            } else {
                lambdaxi = 1;
                lambdaeta = 1;
            }
            c1 = -2 / deltat - lambdaxi * abs(uxi) / (2 * deltaxi) - lambdaeta * abs(ueta) / (2 * deltaeta) - 4 / (hxi * hxi * deltaxi * deltaxi * Re) - 4 / (heta * heta * deltaeta * deltaeta * Re);
            c2 = -2 / deltat + lambdaxi * abs(uxi) / (2 * deltaxi) + lambdaeta * abs(ueta) / (2 * deltaeta) + 4 / (hxi * hxi * deltaxi * deltaxi * Re) + 4 / (heta * heta * deltaeta * deltaeta * Re);
            c3 = - (ueta - lambdaeta * abs(ueta)) / (12 * deltaeta);
            c4 = (2 * ueta - lambdaeta * abs(ueta)) / (3 * deltaeta) - 2 / (heta * heta * deltaeta * deltaeta * Re);
            c5 = -(2 * ueta + lambdaeta * abs(ueta)) / (3 * deltaeta) - 2 / (heta * heta * deltaeta * deltaeta * Re);
            c6 = (ueta + lambdaeta * abs(ueta)) / (12 * deltaeta);
            c7 = - (uxi - lambdaxi * abs(uxi)) / (12 * deltaxi);
            c8 = (2 * uxi - lambdaxi * abs(uxi)) / (3 * deltaxi) - 2 / (hxi * hxi * deltaxi * deltaxi * Re);
            c9 = -(2 * uxi + lambdaxi * abs(uxi)) / (3 * deltaxi) - 2 / (hxi * hxi * deltaxi * deltaxi * Re);
            c10 = (uxi + lambdaxi * abs(uxi)) / (12 * deltaxi);
        }
    }

    /* Calculating new zeta at t + deltat on inner nodes */

    /* Calculating new psi */

    /* Time step */
    t += deltat;
}
