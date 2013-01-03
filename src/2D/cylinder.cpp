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

bool cylinderSpotStain::isOutofRange()
{
    double xip = xi / project->deltaxi;
    double etap = eta / project->deltaeta;

    if ((xip < project->leftboundary + 2) || (xip > project->rightboundary - 2)
        || (etap < project->downboundary + 2) || (etap > project->upboundary - 2)
        || ((etap == 0) && (xip >= project->leftterminal)
            && (xip <= project->rightterminal))) {
        return true;
    } else {
        return false;
    }
}


void cylinderSpotStain::run()
{
    double uxi, ueta;     /* velocity */
    double xip = xi / project->deltaxi;   /* Xi-coordination */
    double etap = eta / project->deltaeta;
    double d1 = xip - floor(xip);
    double d2 = etap - floor(etap);
    double neweta;
    uxi =
        d1 * d2 * project->coordination->access(floor(xip) + 1,
                floor(etap) + 1).uxi + (1 -
                                        d1) * d2 *
        project->coordination->access(floor(xip),
                                      floor(etap) + 1).uxi + d1 * (1 -
                                              d2) *
        project->coordination->access(floor(xip) + 1,
                                      floor(etap)).uxi + (1 - d1) * (1 -
                                              d2) *
        project->coordination->access(floor(xip), floor(etap)).uxi;
    ueta =
        d1 * d2 * project->coordination->access(floor(etap) + 1,
                floor(etap) + 1).ueta + (1 -
                                         d1) *
        d2 * project->coordination->access(floor(etap),
                                           floor(etap) + 1).ueta + d1 * (1 -
                                                   d2) *
        project->coordination->access(floor(etap) + 1,
                                      floor(etap)).ueta + (1 - d1) * (1 -
                                              d2) *
        project->coordination->access(floor(etap), floor(etap)).ueta;
    xi += uxi * project->deltat;
    neweta = eta + ueta * project->deltat;

    if ((xi >= -1) && (xi <= 1) && (eta * neweta <= 0)) {
        /* flow through the cylinder, stay on the boundary */
        eta = 0;
    } else {
        eta = neweta;
    }
}

void cylinderSpotStain::destroychain()
{
    if (next) {
        next->destroychain();
    }

    delete this;
}

cylinderSpotStainSource::cylinderSpotStainSource(cylinderProject * p): project
    (p)
{
    int i;
    step = floor(1 / project->density);
    number =
        ceil((double)(project->upboundary - project->downboundary - 4) / step);
    source = new cylinderSpotStain *[number];

    for (i = 0; i < number; i++) {
        source[i] = NULL;
    }
}

void cylinderSpotStainSource::run()
{
    int i;
    cylinderSpotStain * spot, *tempspot;

    #pragma omp parallel for private(i, spot, tempspot)
    for (i = 0; i < number; i++) {
        spot = source[i];

        while (spot) {
            spot->run();
            spot = spot->next;
        }

        /* insert new spots */
        spot = new cylinderSpotStain(project);
        spot->xi = (project->leftboundary + 2) * project->deltaxi;
        spot->eta = (project->downboundary + 2 + i * step) * project->deltaeta;
        spot->next = source[i];
        source[i] = spot;
        /* delete outofrange spots */
        spot = source[i];

        while (spot->next) {
            if (spot->next->isOutofRange()) {
                tempspot = spot->next;
                spot->next = spot->next->next;
                delete tempspot;
            } else {
                spot = spot->next;
            }
        }
    }
}


void cylinderProject::initialize()
{
    int i, j;
    double g1, g2, g3, g4;
    double x, y;
    cylinderNode * node;

    #pragma omp parallel for private(i, j, g1, g2, g3, g4, x, y, node)

    for (i = downboundary; i <= upboundary; i++) {
        for (j = leftboundary; j <= rightboundary; j++) {
            if ((i == 0) && (j >= leftterminal) && (j <= rightterminal)) {
                /* On the cylinder, do nothing but initialize zeta for future use */
                coordination->access(j, i).zeta = 0;
                coordination->access(j, i).zetat = 0;
                continue;
            }

            node = &coordination->access(j, i);
            node->xi = j * deltaxi;
            node->eta = i * deltaeta;
            node->calculateXY();
            x = node->x;
            y = node->y;

            node->hxi =
                sqrt(1 +
                     (1 + x * x -
                      y * y) / ((x * x + y * y) * (x * x + y * y))) / 2;
            node->heta = node->hxi;

            g1 = 1 / (node->hxi * node->hxi);
            g2 = 1 / (node->heta * node->heta);
            g3 = 0;
            g4 = 0;
            node->b0 =
                2 * g1 / (deltaxi * deltaxi) + 2 * g2 / (deltaeta * deltaeta);
            node->b1 = g1 / (deltaxi * deltaxi) + g3 / (2 * deltaxi);
            node->b2 = g1 / (deltaxi * deltaxi) - g3 / (2 * deltaxi);
            node->b3 = g2 / (deltaeta * deltaeta) + g4 / (2 * deltaeta);
            node->b4 = g2 / (deltaeta * deltaeta) - g4 / (2 * deltaeta);

            /* Initial conditions */
            node->psi = y * (1 - 1 / (x * x + y * y));
            node->zeta = 0;
        }
    }

    //Boundary conditions: left boundary
    #pragma omp parallel for private(i)

    for (i = downboundary; i <= upboundary; i++) {
        coordination->access(leftboundary + 1, i).psi =
            coordination->access(leftboundary + 1, i).y;
        /* zeta is already 0 */
        coordination->access(leftboundary, i).zetat = 0;
        coordination->access(leftboundary + 1, i).zetat = 0;
    }

    //Boundary conditions: right boundary TODO:differential restrict
    #pragma omp parallel for private(i)

    for (i = downboundary; i <= upboundary; i++) {
        coordination->access(rightboundary - 1, i).psi =
            coordination->access(rightboundary - 1, i).y;
        /* zeta is already 0 */
        coordination->access(rightboundary, i).zetat = 0;
        coordination->access(rightboundary - 1, i).zetat = 0;
    }

    //Boundary conditions: up and down boundary
    /* psi is already given */
    /* zeta is already 0 */
    #pragma omp parallel for private(i)

    for (i = leftboundary; i <= rightboundary; i++) {
        coordination->access(i, upboundary).zetat = 0;
        coordination->access(i, upboundary - 1).zetat = 0;
        coordination->access(i, downboundary).zetat = 0;
        coordination->access(i, downboundary + 1).zetat = 0;
    }

    //Boundary conditions: cylinder
    #pragma omp parallel for private(j, node)

    for (j = leftterminal; j <= rightterminal; j++) {
        /* upper half */
        node = &cylinderBoundary->access(j, 1);
        node->xi = j * deltaxi;
        node->x = node->xi;
        node->eta = 0;
        node->y = sqrt(1 - node->x * node->x);
        node->hxi = sqrt(1 + 2 * node->x * node->x) / 2;
        node->heta = node->hxi;
        node->psi = 0;
        node->zeta = 0;

        /* lower half */
        node = &cylinderBoundary->access(j, 0);
        node->xi = j * deltaxi;
        node->x = node->xi;
        node->eta = 0;
        node->y = -sqrt(1 - node->x * node->x);
        node->hxi = sqrt(1 + 2 * node->x * node->x) / 2;
        node->heta = node->hxi;
        node->psi = 0;
        node->zeta = 0;
    }

    /* For convenience, set psi at these two points to 0 */
    coordination->access(leftterminal, 0).psi = 0;
    coordination->access(rightterminal, 0).psi = 0;

    return;
}

void cylinderProject::run()
{
    double vxi, veta;
    double uxi, ueta;
    double hxi, heta;
    double lambdaxi, lambdaeta;
    int i, j;
    int converge;
    cylinderNode * node;

    /* Calculating boundary conditions of zeta */
    #pragma omp parallel for private(j)

    for (j = leftterminal; j <= rightterminal; j++) {
        /* upper half */
        cylinderBoundary->access(j, 1).zeta =
            -2 * (coordination->access(j, 1).psi -
                  0 /* cylinderBoundary->access(j, 1).psi) */) /
            (cylinderBoundary->access(j, 1).heta *
             cylinderBoundary->access(j, 1).heta * deltaeta * deltaeta);
        cylinderBoundary->access(j, 1).zetat =
            cylinderBoundary->access(j, 1).zeta;
        /* lower half */
        cylinderBoundary->access(j, 0).zeta =
            -2 * (coordination->access(j, -1).psi -
                  0 /* cylinderBoundary->access(j, 0).psi) */) /
            (cylinderBoundary->access(j, 0).heta *
             cylinderBoundary->access(j, 0).heta * deltaeta * deltaeta);
        cylinderBoundary->access(j, 0).zetat =
            cylinderBoundary->access(j, 0).zeta;
    }

    /* For convenience, set zeta at these two points */
    coordination->access(leftterminal, 0).zeta =
        (cylinderBoundary->access(leftterminal, 1).zeta +
         cylinderBoundary->access(leftterminal, 0).zeta) / 2;
    coordination->access(rightterminal, 0).zeta =
        (cylinderBoundary->access(rightterminal, 1).zeta +
         cylinderBoundary->access(rightterminal, 0).zeta) / 2;

    /* Calculating new zeta at t + deltat on inner nodes */
    /* Calculating coefficients */
    #pragma omp parallel for private(i, j, node, hxi, heta, vxi, veta, uxi, ueta, lambdaxi, lambdaeta)

    for (i = downboundary + 2; i < upboundary - 1; i++) {
        for (j = leftboundary + 2; j < rightboundary - 1; j++) {
            if ((i <= 1) && (i >= -1) && (j >= leftterminal)
                && (j <= rightterminal)) {
                /* Near or on the cylinder, do nothing here */
                continue;
            }

            node = &coordination->access(j, i);
            hxi = node->hxi;
            heta = node->heta;
            vxi =
                (coordination->access(j, i + 1).psi -
                 coordination->access(j, i - 1).psi) / (2 * heta * deltaeta);
            veta =
                -(coordination->access(j + 1, i).psi -
                  coordination->access(j - 1, i).psi) / (2 * hxi * deltaxi);
            uxi = vxi / hxi;
            ueta = veta / heta;

            if (Re < 1000) {
                lambdaxi = 1 - 1 / exp(abs(uxi));
                lambdaeta = 1 - 1 / exp(abs(ueta));
            } else {
                lambdaxi = 1;
                lambdaeta = 1;
            }

            node->c1 =
                -2 / deltat - lambdaxi * abs(uxi) / (2 * deltaxi) -
                lambdaeta * abs(ueta) / (2 * deltaeta) -
                4 / (hxi * hxi * deltaxi * deltaxi * Re) -
                4 / (heta * heta * deltaeta * deltaeta * Re);
            node->c2 =
                -2 / deltat + lambdaxi * abs(uxi) / (2 * deltaxi) +
                lambdaeta * abs(ueta) / (2 * deltaeta) +
                4 / (hxi * hxi * deltaxi * deltaxi * Re) +
                4 / (heta * heta * deltaeta * deltaeta * Re);
            node->c3 = -(ueta - lambdaeta * abs(ueta)) / (12 * deltaeta);
            node->c4 =
                (2 * ueta - lambdaeta * abs(ueta)) / (3 * deltaeta) -
                2 / (heta * heta * deltaeta * deltaeta * Re);
            node->c5 =
                -(2 * ueta + lambdaeta * abs(ueta)) / (3 * deltaeta) -
                2 / (heta * heta * deltaeta * deltaeta * Re);
            node->c6 = (ueta + lambdaeta * abs(ueta)) / (12 * deltaeta);
            node->c7 = -(uxi - lambdaxi * abs(uxi)) / (12 * deltaxi);
            node->c8 =
                (2 * uxi - lambdaxi * abs(uxi)) / (3 * deltaxi) -
                2 / (hxi * hxi * deltaxi * deltaxi * Re);
            node->c9 =
                -(2 * uxi + lambdaxi * abs(uxi)) / (3 * deltaxi) -
                2 / (hxi * hxi * deltaxi * deltaxi * Re);
            node->c10 = (uxi + lambdaxi * abs(uxi)) / (12 * deltaxi);
        }
    }

    #pragma omp parallel for private(j, hxi, heta, vxi, veta, uxi, ueta, lambdaxi, lambdaeta, node)

    for (j = leftterminal; j <= rightterminal; j++) {
        /* Upper half */
        node = &coordination->access(j, 1);
        hxi = node->hxi;
        heta = node->heta;
        vxi =
            (coordination->access(j, 2).psi -
             cylinderBoundary->access(j, 1).psi) / (2 * heta * deltaeta);
        veta =
            -(coordination->access(j + 1, 1).psi -
              coordination->access(j - 1, 1).psi) / (2 * hxi * deltaxi);
        uxi = vxi / hxi;
        ueta = veta / heta;

        if (Re < 1000) {
            lambdaxi = 1 - 1 / exp(abs(uxi));
            lambdaeta = 1 - 1 / exp(abs(ueta));
        } else {
            lambdaxi = 1;
            lambdaeta = 1;
        }

        node->c1 =
            -2 / deltat - lambdaxi * abs(uxi) / (2 * deltaxi) -
            (-ueta) / (2 * deltaeta) -
            4 / (hxi * hxi * deltaxi * deltaxi * Re) -
            4 / (heta * heta * deltaeta * deltaeta * Re);
        node->c2 =
            -2 / deltat + lambdaxi * abs(uxi) / (2 * deltaxi) +
            (-ueta) / (2 * deltaeta) +
            4 / (hxi * hxi * deltaxi * deltaxi * Re) +
            4 / (heta * heta * deltaeta * deltaeta * Re);
        node->c3 = -(ueta - (-ueta)) / (12 * deltaeta);
        node->c4 =
            (2 * ueta - (-ueta)) / (3 * deltaeta) -
            2 / (heta * heta * deltaeta * deltaeta * Re);
        node->c5 =
            -(2 * ueta + (-ueta)) / (3 * deltaeta) -
            2 / (heta * heta * deltaeta * deltaeta * Re);
        node->c6 = (ueta + (-ueta)) / (12 * deltaeta);
        node->c7 = -(uxi - lambdaxi * abs(uxi)) / (12 * deltaxi);
        node->c8 =
            (2 * uxi - lambdaxi * abs(uxi)) / (3 * deltaxi) -
            2 / (hxi * hxi * deltaxi * deltaxi * Re);
        node->c9 =
            -(2 * uxi + lambdaxi * abs(uxi)) / (3 * deltaxi) -
            2 / (hxi * hxi * deltaxi * deltaxi * Re);
        node->c10 = (uxi + lambdaxi * abs(uxi)) / (12 * deltaxi);

        /* Lower half */
        node = &coordination->access(j, -1);
        hxi = node->hxi;
        heta = node->heta;
        vxi =
            (cylinderBoundary->access(j, 0).psi -
             coordination->access(j, -2).psi) / (2 * heta * deltaeta);
        veta =
            -(coordination->access(j + 1, -1).psi -
              coordination->access(j - 1, -1).psi) / (2 * hxi * deltaxi);
        uxi = vxi / hxi;
        ueta = veta / heta;

        if (Re < 1000) {
            lambdaxi = 1 - 1 / exp(abs(uxi));
            lambdaeta = 1 - 1 / exp(abs(ueta));
        } else {
            lambdaxi = 1;
            lambdaeta = 1;
        }

        node->c1 =
            -2 / deltat - lambdaxi * abs(uxi) / (2 * deltaxi) -
            ueta / (2 * deltaeta) -
            4 / (hxi * hxi * deltaxi * deltaxi * Re) -
            4 / (heta * heta * deltaeta * deltaeta * Re);
        node->c2 =
            -2 / deltat + lambdaxi * abs(uxi) / (2 * deltaxi) +
            ueta / (2 * deltaeta) +
            4 / (hxi * hxi * deltaxi * deltaxi * Re) +
            4 / (heta * heta * deltaeta * deltaeta * Re);
        node->c3 = -(ueta - ueta) / (12 * deltaeta);
        node->c4 =
            (2 * ueta - ueta) / (3 * deltaeta) -
            2 / (heta * heta * deltaeta * deltaeta * Re);
        node->c5 =
            -(2 * ueta + ueta) / (3 * deltaeta) -
            2 / (heta * heta * deltaeta * deltaeta * Re);
        node->c6 = (ueta + ueta) / (12 * deltaeta);
        node->c7 = -(uxi - lambdaxi * abs(uxi)) / (12 * deltaxi);
        node->c8 =
            (2 * uxi - lambdaxi * abs(uxi)) / (3 * deltaxi) -
            2 / (hxi * hxi * deltaxi * deltaxi * Re);
        node->c9 =
            -(2 * uxi + lambdaxi * abs(uxi)) / (3 * deltaxi) -
            2 / (hxi * hxi * deltaxi * deltaxi * Re);
        node->c10 = (uxi + lambdaxi * abs(uxi)) / (12 * deltaxi);
    }

    /* left stagnation point */
    j = leftterminal - 1;
    i = 0;
    node = &coordination->access(j, i);
    hxi = node->hxi;
    heta = node->heta;
    vxi =
        (coordination->access(j, i + 1).psi -
         coordination->access(j, i - 1).psi) / (2 * heta * deltaeta);
    veta =
        -(coordination->access(j + 1, i).psi -
          coordination->access(j - 1, i).psi) / (2 * hxi * deltaxi);
    uxi = vxi / hxi;
    ueta = veta / heta;

    if (Re < 1000) {
        lambdaxi = 1 - 1 / exp(abs(uxi));
        lambdaeta = 1 - 1 / exp(abs(ueta));
    } else {
        lambdaxi = 1;
        lambdaeta = 1;
    }

    node->c1 =
        -2 / deltat - uxi / (2 * deltaxi) -
        lambdaeta * abs(ueta) / (2 * deltaeta) -
        4 / (hxi * hxi * deltaxi * deltaxi * Re) -
        4 / (heta * heta * deltaeta * deltaeta * Re);
    node->c2 =
        -2 / deltat + uxi / (2 * deltaxi) +
        lambdaeta * abs(ueta) / (2 * deltaeta) +
        4 / (hxi * hxi * deltaxi * deltaxi * Re) +
        4 / (heta * heta * deltaeta * deltaeta * Re);
    node->c3 = -(ueta - lambdaeta * abs(ueta)) / (12 * deltaeta);
    node->c4 =
        (2 * ueta - lambdaeta * abs(ueta)) / (3 * deltaeta) -
        2 / (heta * heta * deltaeta * deltaeta * Re);
    node->c5 =
        -(2 * ueta + lambdaeta * abs(ueta)) / (3 * deltaeta) -
        2 / (heta * heta * deltaeta * deltaeta * Re);
    node->c6 = (ueta + lambdaeta * abs(ueta)) / (12 * deltaeta);
    node->c7 = -(uxi - uxi) / (12 * deltaxi);
    node->c8 =
        (2 * uxi - uxi) / (3 * deltaxi) - 2 / (hxi * hxi * deltaxi * deltaxi * Re);
    node->c9 =
        -(2 * uxi + uxi) / (3 * deltaxi) - 2 / (hxi * hxi * deltaxi * deltaxi * Re);
    node->c10 = (uxi + uxi) / (12 * deltaxi);
    /* right stagnation point */
    j = rightterminal + 1;
    i = 0;
    node = &coordination->access(j, i);
    hxi = node->hxi;
    heta = node->heta;
    vxi =
        (coordination->access(j, i + 1).psi -
         coordination->access(j, i - 1).psi) / (2 * heta * deltaeta);
    veta =
        -(coordination->access(j + 1, i).psi -
          coordination->access(j - 1, i).psi) / (2 * hxi * deltaxi);
    uxi = vxi / hxi;
    ueta = veta / heta;

    if (Re < 1000) {
        lambdaxi = 1 - 1 / exp(abs(uxi));
        lambdaeta = 1 - 1 / exp(abs(ueta));
    } else {
        lambdaxi = 1;
        lambdaeta = 1;
    }

    node->c1 =
        -2 / deltat - (-uxi) / (2 * deltaxi) -
        lambdaeta * abs(ueta) / (2 * deltaeta) -
        4 / (hxi * hxi * deltaxi * deltaxi * Re) -
        4 / (heta * heta * deltaeta * deltaeta * Re);
    node->c2 =
        -2 / deltat + (-uxi) / (2 * deltaxi) +
        lambdaeta * abs(ueta) / (2 * deltaeta) +
        4 / (hxi * hxi * deltaxi * deltaxi * Re) +
        4 / (heta * heta * deltaeta * deltaeta * Re);
    node->c3 = -(ueta - lambdaeta * abs(ueta)) / (12 * deltaeta);
    node->c4 =
        (2 * ueta - lambdaeta * abs(ueta)) / (3 * deltaeta) -
        2 / (heta * heta * deltaeta * deltaeta * Re);
    node->c5 =
        -(2 * ueta + lambdaeta * abs(ueta)) / (3 * deltaeta) -
        2 / (heta * heta * deltaeta * deltaeta * Re);
    node->c6 = (ueta + lambdaeta * abs(ueta)) / (12 * deltaeta);
    node->c7 = -(uxi - (-uxi)) / (12 * deltaxi);
    node->c8 =
        (2 * uxi - (-uxi)) / (3 * deltaxi) -
        2 / (hxi * hxi * deltaxi * deltaxi * Re);
    node->c9 =
        -(2 * uxi + (-uxi)) / (3 * deltaxi) -
        2 / (hxi * hxi * deltaxi * deltaxi * Re);
    node->c10 = (uxi + (-uxi)) / (12 * deltaxi);

    /* copy zeta to zetat to use as guessing solve */
    #pragma omp parallel for private(j, i)

    for (j = downboundary + 2; j < upboundary - 1; j++) {
        for (i = leftboundary + 2; i < rightboundary - 1; i++) {
            coordination->access(i, j).zetat = coordination->access(i, j).zeta;
        }
    }


    /* recursive calculation */
    converge = 30;

    while (converge) {
        //TODO:converge
        /* One step */
        #pragma omp parallel for private(j, i, node)
        for (j = downboundary + 2; j < upboundary - 1; j++) {
            for (i = leftboundary + 2; i < rightboundary - 1; i++) {
                if ((j <= 2) && (j >= -2) && (i >= leftterminal)
                    && (i <= rightterminal)) {
                    /* Near or on the cylinder, do nothing here */
                    continue;
                }

                node = &coordination->access(i, j);
                node->newzetat =
                    (node->c2 * node->zeta +
                     node->c3 * (coordination->access(i, j + 2).zetat +
                                 coordination->access(i,
                                                      j + 2).zeta) +
                     node->c4 * (coordination->access(i, j + 1).zetat +
                                 coordination->access(i,
                                                      j + 1).zeta) +
                     node->c5 * (coordination->access(i, j - 1).zetat +
                                 coordination->access(i,
                                                      j - 1).zeta) +
                     node->c6 * (coordination->access(i, j - 2).zetat +
                                 coordination->access(i,
                                                      j - 2).zeta) +
                     node->c7 * (coordination->access(i + 2, j).zetat +
                                 coordination->access(i + 2,
                                                      j).zeta) +
                     node->c8 * (coordination->access(i + 1, j).zetat +
                                 coordination->access(i + 1,
                                                      j).zeta) +
                     node->c9 * (coordination->access(i - 1, j).zetat +
                                 coordination->access(i - 1,
                                                      j).zeta) +
                     node->c10 * (coordination->access(i - 2, j).zetat +
                                  coordination->access(i - 2, j).zeta)) / node->c1;
            }
        }

        /* j == +-2 */
        j = 2;
        #pragma omp parallel for private(i, node)

        for (i = leftterminal; i <= rightterminal; i++) {
            node = &coordination->access(i, j);
            node->newzetat =
                (node->c2 * node->zeta +
                 node->c3 * (coordination->access(i, j + 2).zetat +
                             coordination->access(i,
                                                  j + 2).zeta) +
                 node->c4 * (coordination->access(i, j + 1).zetat +
                             coordination->access(i,
                                                  j + 1).zeta) +
                 node->c5 * (coordination->access(i, j - 1).zetat +
                             coordination->access(i,
                                                  j - 1).zeta) +
                 node->c6 * (cylinderBoundary->access(i, 1).zetat +
                             cylinderBoundary->access(i,
                                     1).zeta) +
                 node->c7 * (coordination->access(i + 2, j).zetat +
                             coordination->access(i + 2,
                                                  j).zeta) +
                 node->c8 * (coordination->access(i + 1, j).zetat +
                             coordination->access(i + 1,
                                                  j).zeta) +
                 node->c9 * (coordination->access(i - 1, j).zetat +
                             coordination->access(i - 1,
                                                  j).zeta) +
                 node->c10 * (coordination->access(i - 2, j).zetat +
                              coordination->access(i - 2, j).zeta)) / node->c1;
        }

        j = -2;
        #pragma omp parallel for private(i, node)

        for (i = leftterminal; i <= rightterminal; i++) {
            node = &coordination->access(i, j);
            node->newzetat =
                (node->c2 * node->zeta +
                 node->c3 * (cylinderBoundary->access(i, 0).zetat +
                             cylinderBoundary->access(i,
                                     0).zeta) +
                 node->c4 * (coordination->access(i, j + 1).zetat +
                             coordination->access(i,
                                                  j + 1).zeta) +
                 node->c5 * (coordination->access(i, j - 1).zetat +
                             coordination->access(i,
                                                  j - 1).zeta) +
                 node->c6 * (coordination->access(i, j - 2).zetat +
                             coordination->access(i,
                                                  j - 2).zeta) +
                 node->c7 * (coordination->access(i + 2, j).zetat +
                             coordination->access(i + 2,
                                                  j).zeta) +
                 node->c8 * (coordination->access(i + 1, j).zetat +
                             coordination->access(i + 1,
                                                  j).zeta) +
                 node->c9 * (coordination->access(i - 1, j).zetat +
                             coordination->access(i - 1,
                                                  j).zeta) +
                 node->c10 * (coordination->access(i - 2, j).zetat +
                              coordination->access(i - 2, j).zeta)) / node->c1;
        }

        /* j == +- 1 */
        j = 1;
        #pragma omp parallel for private(i, node)

        for (i = leftterminal; i <= rightterminal; i++) {
            node = &coordination->access(i, j);
            node->newzetat =
                    (node->c2 * node->zeta +
                     node->c3 * (coordination->access(i, j + 2).zetat +
                                 coordination->access(i,
                                                      j + 2).zeta) +
                     node->c4 * (coordination->access(i, j + 1).zetat +
                                 coordination->access(i,
                                                      j + 1).zeta) +
                     node->c5 * (coordination->access(i, j - 1).zetat +
                                 coordination->access(i,
                                                      j - 1).zeta) +
                     node->c6 * (coordination->access(i, j - 2).zetat +
                                 coordination->access(i,
                                                      j - 2).zeta) +
                     node->c7 * (coordination->access(i + 2, j).zetat +
                                 coordination->access(i + 2,
                                                      j).zeta) +
                     node->c8 * (coordination->access(i + 1, j).zetat +
                                 coordination->access(i + 1,
                                                      j).zeta) +
                     node->c9 * (coordination->access(i - 1, j).zetat +
                                 coordination->access(i - 1,
                                                      j).zeta) +
                     node->c10 * (coordination->access(i - 2, j).zetat +
                                  coordination->access(i - 2, j).zeta)) / node->c1;
        }

        j = -1;
        #pragma omp parallel for private(i, node)

        for (i = leftterminal; i <= rightterminal; i++) {
            node = &coordination->access(i, j);
            node->newzetat =
                (node->c2 * node->zeta +
                 node->c4 * (cylinderBoundary->access(i, 0).zetat +
                             cylinderBoundary->access(i,
                                     0).zeta) +
                 node->c5 * (coordination->access(i, j - 1).zetat +
                             coordination->access(i,
                                                  j - 1).zeta) +
                 node->c6 * (coordination->access(i, j - 2).zetat +
                             coordination->access(i,
                                                  j - 2).zeta) +
                 node->c7 * (coordination->access(i + 2, j).zetat +
                             coordination->access(i + 2,
                                                  j).zeta) +
                 node->c8 * (coordination->access(i + 1, j).zetat +
                             coordination->access(i + 1,
                                                  j).zeta) +
                 node->c9 * (coordination->access(i - 1, j).zetat +
                             coordination->access(i - 1,
                                                  j).zeta) +
                 node->c10 * (coordination->access(i - 2, j).zetat +
                              coordination->access(i - 2, j).zeta)) / node->c1;
        }

        /* End of one step */

        /* flush */
        #pragma omp parallel for private(j, i)

        for (j = downboundary + 2; j < upboundary - 1; j++) {
            for (i = leftboundary + 2; i < rightboundary - 1; i++) {
                coordination->access(i, j).zetat =
                    coordination->access(i, j).newzetat;
            }
        }

        converge -= 1;
    }

    /* flush zetat back to zeta */
    #pragma omp parallel for private(j, i)

    for (j = downboundary + 2; j < upboundary - 1; j++) {
        for (i = leftboundary + 2; i < rightboundary - 1; i++) {
            coordination->access(i, j).zeta = coordination->access(i, j).zetat;
        }
    }

    /* Time step */
    t += deltat;

    /* Calculating new psi */
    converge = 30;

    while (converge) {
        /* One step */
        #pragma omp parallel for private(j, i, node)
        for (j = downboundary + 2; j < upboundary - 1; j++) {
            for (i = leftboundary + 2; i < rightboundary - 1; i++) {
                if ((j == 0) && (i >= leftterminal) && (i <= rightterminal)) {
                    /* On the cylinder, do nothing here */
                    continue;
                }

                node = &coordination->access(i, j);
                node->newpsi =
                    (1 - omega) * node->psi +
                    omega * (node->b1 *
                             coordination->access(i + 1,
                                                  j).psi +
                             node->b2 * coordination->access(i - 1,
                                     j).psi +
                             node->b3 * coordination->access(i,
                                     j + 1).psi +
                             node->b4 * coordination->access(i,
                                     j - 1).psi +
                             node->zeta) / node->b0;
            }
        }

        /* End of one step */

        /* flush */
        #pragma omp parallel for private(j, i)

        for (j = downboundary + 2; j < upboundary - 1; j++) {
            for (i = leftboundary + 2; i < rightboundary - 1; i++) {
                coordination->access(i, j).psi =
                    coordination->access(i, j).newpsi;
            }
        }

        converge -= 1;
    }

    /* Calculating velocity */
    #pragma omp parallel for private(j, i, node)

    for (j = downboundary + 2; j < upboundary - 1; j++) {
        for (i = leftboundary + 2; i < rightboundary - 1; i++) {
            if ((j == 0) && (i >= leftterminal) && (i <= rightterminal)) {
                /* On the cylinder, do nothing here */
                continue;
            }

            node = &coordination->access(i, j);
            node->uxi =
                (coordination->access(i, j + 1).psi -
                 coordination->access(i, j -
                                      1).psi) / (2 * deltaeta * node->hxi *
                                                 node->heta);
            node->ueta =
                -(coordination->access(i + 1, j).psi -
                  coordination->access(i - 1,
                                       j).psi) / (2 * deltaxi * node->hxi *
                                                  node->heta);
        }
    }

}

void cylinderProject::spotstainrun()
{
    source->run();
}
