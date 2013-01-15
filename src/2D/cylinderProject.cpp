#include <cmath>
#include <cstdarg>
#include <fstream>
#include "solver.h"
#include "cylinder.h"

using std::abs;

cylinderProject::cylinderProject(int l = -100, int r = 400, int u = 100, int d = -100, double dens = 0.1, double dxi = 0.1, double deta = 0.1, double dt = 0.1, double rey = 40): leftboundary(l), rightboundary(r), upboundary(u), downboundary(d),
        density(dens), deltaxi(dxi), deltaeta(deta), deltat(dt)
{
    t = 0;
    Re = 2 * rey;
    leftterminal = -1 / deltaxi;
    rightterminal = 1 / deltaxi;
    coordination = new cylinderCoordinate(l, r, u, d);
    cylinderBoundary = new cylinderCoordinate(leftterminal, rightterminal, 1, 0);
    source = new cylinderSpotStainSource(this);
}

cylinderProject::cylinderProject(const char* location)
{   
    int i,j;
    cylinderNode* node;
    std::ifstream file(location);

    /* basic parameters */
    file >> t;
    file >> Re;
    file >> leftboundary >> rightboundary >> upboundary >> downboundary;
    file >> leftterminal >> rightterminal;
    file >> density;
    file >> deltaxi;
    file >> deltaeta;
    file >> deltat;

    coordination = new cylinderCoordinate(leftboundary, rightboundary, upboundary, downboundary);
    cylinderBoundary = new cylinderCoordinate(leftterminal, rightterminal, 1, 0);
    source = new cylinderSpotStainSource(this);

    initialize();
    
    /* node properties */
    for (i=downboundary;i<= upboundary;i++){
        for (j=leftboundary;j<=rightboundary;j++){
            node = &coordination->access(i,j);
            file >> node->zeta;
            file >> node->psi;
        }
    }

    /* node on cylinder */
    for (j=0;j<=1;j++){
        for (i=leftterminal;i<= rightterminal;i++){
            node = &cylinderBoundary->access(i,j);
            file >> node->zeta;
            file >> node->psi;
        }
    }

    calculateVelocity();
    
    file.close();
}

cylinderProject::~cylinderProject() {
    delete coordination;
    delete cylinderBoundary;
    delete source;
}

int cylinderProject::psiConvert(int i, int j) 
{
    return (j - (downboundary + 1)) * (rightboundary - leftboundary - 1) + i - (leftboundary + 1);
}

int cylinderProject::zetaConvert(int i, int j)
{
    return (j - downboundary) * (rightboundary - leftboundary + 1) + i - leftboundary;
}

bool cylinderProject::OnBoundary(int i, int j)
{
    if ((i <= (leftboundary+1)) || (i >= (rightboundary -1)) || (j <= (downboundary+1)) || (j>=(upboundary-1)) || ((j == 0) && (i >= leftterminal) && (i <= rightterminal))) {
        return true;
    } else {
        return false;
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
                /* On the cylinder, do nothing but initialize psi and zeta for future use */
                coordination->access(j, i).psi = 0;
                coordination->access(j, i).zeta = 0;
                continue;
            }

            node = &coordination->access(j, i);
            node->xi = j * deltaxi;
            node->eta = i * deltaeta;
            node->calculateXY();
            x = node->x;
            y = node->y;

            node->hxi = 2 * (x*x+y*y) / sqrt((x*x+y*y)*(x*x+y*y)-2*(x*x-y*y)+1);
               
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
    }

    //Boundary conditions: right boundary 
    #pragma omp parallel for private(i)

    for (i = downboundary; i <= upboundary; i++) {
        coordination->access(rightboundary - 1, i).psi =
            coordination->access(rightboundary - 1, i).y;
        /* zeta is already 0 */
    }

    //Boundary conditions: up and down boundary
    /* psi is already given */
    /* zeta is already 0 */

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

    return;
}

void cylinderProject::calculateBoundaryZeta()
{
    int i;
    /* Calculating boundary conditions of zeta */
    #pragma omp parallel for private(i)

    for (i = leftterminal; i <= rightterminal; i++) {
        /* upper half */
        cylinderBoundary->access(i, 1).zeta =
            -2 * (coordination->access(i, 1).psi -
                  0 /* cylinderBoundary->access(i, 1).psi) */) /
            (cylinderBoundary->access(i, 1).heta *
             cylinderBoundary->access(i, 1).heta * deltaeta * deltaeta);
        /* lower half */
        cylinderBoundary->access(i, 0).zeta =
            -2 * (coordination->access(i, -1).psi -
                  0 /* cylinderBoundary->access(i, 0).psi) */) /
            (cylinderBoundary->access(i, 0).heta *
             cylinderBoundary->access(i, 0).heta * deltaeta * deltaeta);
    }

    /* For convenience, set zeta at these two points */
    coordination->access(leftterminal, 0).zeta =
        (cylinderBoundary->access(leftterminal, 1).zeta +
         cylinderBoundary->access(leftterminal, 0).zeta) / 2;
    coordination->access(rightterminal, 0).zeta =
        (cylinderBoundary->access(rightterminal, 1).zeta +
         cylinderBoundary->access(rightterminal, 0).zeta) / 2;
}

void cylinderProject::calculateNewZeta()
{
    double vxi, veta;
    double uxi, ueta;
    double hxi, heta;
    double lambdaxi, lambdaeta;
    int i, j;
    cylinderNode * node;
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
    /* End of calculating coeffiecients */

    /* Matrix solver */
    int n = (rightboundary - leftboundary + 1) * (upboundary - downboundary + 1);
    int nz = 9 * n;
    double *b = new double[n];
    int line = 0;
    Solver* eql = new Solver(n,nz,b);
    for (j = downboundary; j <= upboundary; j++){
        for (i = leftboundary; i <= rightboundary; i++){
            if (OnBoundary(i,j)) {
                node = &coordination->access(i,j);
                eql->input(line,zetaConvert(i,j),1);
                eql->input(0, 0, 0);
                eql->input(0, 0, 0);
                eql->input(0, 0, 0);
                eql->input(0, 0, 0);
                eql->input(0, 0, 0);
                eql->input(0, 0, 0);
                eql->input(0, 0, 0);
                eql->input(0, 0, 0);
                b[line] = node->zeta;
                line += 1;
                continue;
            }
            if ((i>=leftterminal) && (i<=rightterminal)) {
                if (j==2){
                    node = &coordination->access(i,j);
                    eql->input(line,zetaConvert(i,j),node->c1);
                    eql->input(line,zetaConvert(i,j+2),-node->c3);
                    eql->input(line,zetaConvert(i,j+1),-node->c4);
                    eql->input(line,zetaConvert(i,j-1),-node->c5);
                    eql->input(0,0,0);
                    eql->input(line,zetaConvert(i+2,j),-node->c7);
                    eql->input(line,zetaConvert(i+1,j),-node->c8);
                    eql->input(line,zetaConvert(i-1,j),-node->c9);
                    eql->input(line,zetaConvert(i-2,j),-node->c10);
                    b[line] = node->c2 * node->zeta + node->c3*coordination->access(i,j+2).zeta + node->c4*coordination->access(i,j+1).zeta + node->c5*coordination->access(i,j-1).zeta + node->c6*cylinderBoundary->access(i,1).zeta*2 + node->c7*coordination->access(i+2,j).zeta + node->c8*coordination->access(i+1,j).zeta + node->c9*coordination->access(i-1,j).zeta + node->c10*coordination->access(i-2,j).zeta;
                    line += 1;
                    continue;
                }
                if (j==-2){
                    node = &coordination->access(i,j);
                    eql->input(line,zetaConvert(i,j),node->c1);
                    eql->input(0,0,0);
                    eql->input(line,zetaConvert(i,j+1),-node->c4);
                    eql->input(line,zetaConvert(i,j-1),-node->c5);
                    eql->input(line,zetaConvert(i,j-2),-node->c6);
                    eql->input(line,zetaConvert(i+2,j),-node->c7);
                    eql->input(line,zetaConvert(i+1,j),-node->c8);
                    eql->input(line,zetaConvert(i-1,j),-node->c9);
                    eql->input(line,zetaConvert(i-2,j),-node->c10);
                    b[line] = node->c2 * node->zeta + node->c3*cylinderBoundary->access(i,0).zeta*2 + node->c4*coordination->access(i,j+1).zeta + node->c5*coordination->access(i,j-1).zeta + node->c6*coordination->access(i,j-2).zeta + node->c7*coordination->access(i+2,j).zeta + node->c8*coordination->access(i+1,j).zeta + node->c9*coordination->access(i-1,j).zeta + node->c10*coordination->access(i-2,j).zeta;
                    line += 1;
                    continue;
                }
                if (j==1){
                    node = &coordination->access(i,j);
                    eql->input(line,zetaConvert(i,j),node->c1);
                    eql->input(line,zetaConvert(i,j+2),-node->c3);
                    eql->input(line,zetaConvert(i,j+1),-node->c4);
                    eql->input(0,0,0);
                    eql->input(0,0,0);
                    eql->input(line,zetaConvert(i+2,j),-node->c7);
                    eql->input(line,zetaConvert(i+1,j),-node->c8);
                    eql->input(line,zetaConvert(i-1,j),-node->c9);
                    eql->input(line,zetaConvert(i-2,j),-node->c10);
                    b[line] = node->c2 * node->zeta + node->c3*coordination->access(i,j+2).zeta + node->c4*coordination->access(i,j+1).zeta + node->c5*cylinderBoundary->access(i,1).zeta*2 + node->c6*cylinderBoundary->access(i,1).zeta*2 + node->c7*coordination->access(i+2,j).zeta + node->c8*coordination->access(i+1,j).zeta + node->c9*coordination->access(i-1,j).zeta + node->c10*coordination->access(i-2,j).zeta;
                    line += 1;
                    continue;
                }
                if (j==-1){
                    node = &coordination->access(i,j);
                    eql->input(line,zetaConvert(i,j),node->c1);
                    eql->input(0,0,0);
                    eql->input(0,0,0);
                    eql->input(line,zetaConvert(i,j-1),-node->c5);
                    eql->input(line,zetaConvert(i,j-2),-node->c6);
                    eql->input(line,zetaConvert(i+2,j),-node->c7);
                    eql->input(line,zetaConvert(i+1,j),-node->c8);
                    eql->input(line,zetaConvert(i-1,j),-node->c9);
                    eql->input(line,zetaConvert(i-2,j),-node->c10);
                    b[line] = node->c2 * node->zeta + node->c3*cylinderBoundary->access(i,0).zeta*2 + node->c4*cylinderBoundary->access(i,0).zeta*2 + node->c5*coordination->access(i,j-1).zeta + node->c6*coordination->access(i,j-2).zeta + node->c7*coordination->access(i+2,j).zeta + node->c8*coordination->access(i+1,j).zeta + node->c9*coordination->access(i-1,j).zeta + node->c10*coordination->access(i-2,j).zeta;
                    line += 1;
                    continue;
                }
            }

            node = &coordination->access(i,j);
            eql->input(line,zetaConvert(i,j),node->c1);
            eql->input(line,zetaConvert(i,j+2),-node->c3);
            eql->input(line,zetaConvert(i,j+1),-node->c4);
            eql->input(line,zetaConvert(i,j-1),-node->c5);
            eql->input(line,zetaConvert(i,j-2),-node->c6);
            eql->input(line,zetaConvert(i+2,j),-node->c7);
            eql->input(line,zetaConvert(i+1,j),-node->c8);
            eql->input(line,zetaConvert(i-1,j),-node->c9);
            eql->input(line,zetaConvert(i-2,j),-node->c10);
            b[line] = node->c2 * node->zeta + node->c3*coordination->access(i,j+2).zeta + node->c4*coordination->access(i,j+1).zeta + node->c5*coordination->access(i,j-1).zeta + node->c6*coordination->access(i,j-2).zeta + node->c7*coordination->access(i+2,j).zeta + node->c8*coordination->access(i+1,j).zeta + node->c9*coordination->access(i-1,j).zeta + node->c10*coordination->access(i-2,j).zeta;
            line += 1;
        }
    }
    eql->solve();
    int counter = 0;
    for (j = downboundary; j <= upboundary; j++) {
        for (i = leftboundary; i <= rightboundary ; i++) {
            coordination->access(i,j).zeta = eql->getSolution()[counter];
            counter += 1;
        }
    }

    /* absolete
    #pragma omp parallel for private(j, i)

    for (j = downboundary + 2; j < upboundary - 1; j++) {
        for (i = leftboundary + 2; i < rightboundary - 1; i++) {
            coordination->access(i, j).zetat = coordination->access(i, j).zeta;
        }
    }


    converge = 30;

    while (converge) {
        //TODO:converge
        #pragma omp parallel for private(j, i, node)
        for (j = downboundary + 2; j < upboundary - 1; j++) {
            for (i = leftboundary + 2; i < rightboundary - 1; i++) {
                if ((j <= 2) && (j >= -2) && (i >= leftterminal)
                    && (i <= rightterminal)) {
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
                     node->c5 * (cylinderBoundary->access(i, 1).zetat +
                                 cylinderBoundary->access(i, 1).zeta) +
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


        #pragma omp parallel for private(j, i)

        for (j = downboundary + 2; j < upboundary - 1; j++) {
            for (i = leftboundary + 2; i < rightboundary - 1; i++) {
                coordination->access(i, j).zetat =
                    coordination->access(i, j).newzetat;
            }
        }

        converge -= 1;
    }
    */

    /* flush zetat back to zeta 
    #pragma omp parallel for private(j, i)

    for (j = downboundary + 2; j < upboundary - 1; j++) {
        for (i = leftboundary + 2; i < rightboundary - 1; i++) {
            coordination->access(i, j).zeta = coordination->access(i, j).zetat;
        }
    } */
    delete [] b;
    delete eql;
}

void cylinderProject::timeStep()
{
    /* Time step */
    t += deltat;
}

void cylinderProject::calculateNewPsi()
{
    int i, j;
    cylinderNode * node;
    /* Calculating new psi */
    int n = (rightboundary - leftboundary - 1) * (upboundary - downboundary - 1);
    int nz = 5 * n;
    double *b = new double[n];
    int line = 0;
    Solver *eql = new Solver(n,nz,b);
    for (j = downboundary + 1; j < upboundary ; j++) {
        for (i = leftboundary + 1; i < rightboundary ; i++) {
            if (OnBoundary(i, j)) {
                node = &coordination->access(i, j);
                eql->input(line, psiConvert(i, j), 1);
                eql->input(0, 0, 0);
                eql->input(0, 0, 0);
                eql->input(0, 0, 0);
                eql->input(0, 0, 0);
                b[line] = node->psi;
                line += 1;
                continue;
            }

            node = &coordination->access(i, j);
            eql->input(line, psiConvert(i, j), node->b0);
            eql->input(line, psiConvert(i+1,j), -node->b1);
            eql->input(line, psiConvert(i-1,j), -node->b2);
            eql->input(line, psiConvert(i,j+1), -node->b3);
            eql->input(line, psiConvert(i,j-1), -node->b4);
            b[line] = node->zeta;
            line += 1;
        }
    }
    eql->solve();           
    int counter = 0;
    for (j = downboundary + 1; j < upboundary; j++) {
        for (i = leftboundary + 1; i < rightboundary ; i++) {
            coordination->access(i,j).psi = eql->getSolution()[counter];
            counter += 1;
        }
    }

    /* absolete */
    /*converge = 30;

    while (converge) {
        #pragma omp parallel for private(j, i, node)
        for (j = downboundary + 2; j < upboundary - 1; j++) {
            for (i = leftboundary + 2; i < rightboundary - 1; i++) {
                if ((j == 0) && (i >= leftterminal) && (i <= rightterminal)) {
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


        #pragma omp parallel for private(j, i)

        for (j = downboundary + 2; j < upboundary - 1; j++) {
            for (i = leftboundary + 2; i < rightboundary - 1; i++) {
                coordination->access(i, j).psi =
                    coordination->access(i, j).newpsi;
            }
        }

        converge -= 1;
    }
    */
    delete [] b;
    delete eql;
}

void cylinderProject::calculateVelocity()
{
    int i, j;
    cylinderNode * node;
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

void cylinderProject::setDensity(double dens) {
    density = dens;
    delete source;
    source = new cylinderSpotStainSource(this);
}

DataVariant * cylinderProject::getData(Project::DataType type, ...)
{
    cylinderDataVariant* data;
    va_list ap;
    switch (type){
        case Project::TimeType :
            data = new cylinderDataVariant(type, &t);
            return data;
        case Project::NumberType :
            data = new cylinderDataVariant(type, &source->getNumber());
            return data;
        case Project::PsiType :
            va_start(ap, type);
            data = new cylinderDataVariant(type, &coordination->access(va_arg(ap, int), va_arg(ap, int)));
            va_end(ap);
            return data;
        case Project::SpotType :
            va_start(ap, type);
            data = new cylinderDataVariant(type, source->getLine(va_arg(ap,int)));
            va_end(ap);
            return data;
        default :
            /* Wrong Type */
            return NULL;
        }
}

void cylinderProject::run()
{
    this->calculateBoundaryZeta();
    this->calculateNewZeta();
    this->timeStep();
    this->calculateNewPsi();
    this->calculateVelocity();
}

void cylinderProject::spotstainrun()
{
    source->run();
}

bool cylinderProject::dumptofile(const char* location)
{
    int i,j;
    cylinderNode* node;
    std::ofstream file(location);
    if ( !file ) {
        return false;
    }

    /* basic parameters */
    file << t << ' ';
    file << Re << ' ';
    file << leftboundary << ' ' << rightboundary << ' ' << upboundary << ' ' << downboundary << ' ';
    file << leftterminal << ' ' << rightterminal << ' ';
    file << density << ' ';
    file << deltaxi << ' ';
    file << deltaeta << ' ';
    file << deltat << std::endl;
    
    /* node properties */
    for (i=downboundary;i<= upboundary;i++){
        for (j=leftboundary;j<=rightboundary;j++){
            node = &coordination->access(i,j);
            file << node->zeta << ' ';
            file << node->psi << ' ';
        }
    }

    file << std::endl;

    /* node on cylinder */
    for (j=0;j<=1;j++){
        for (i=leftterminal;i<= rightterminal;i++){
            node = &cylinderBoundary->access(i,j);
            file << node->zeta << ' ';
            file << node->psi << ' ';
        }
    }
    
    file.close();

    /* successfully dumped */
    return true;
}

