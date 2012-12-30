#ifndef _cylinder
#define _cylinder

#include "commontypes.h"

class cylinderNode : public Node {
public:
    double hxi;   /* lame coefficient */
    double heta;
    double b0, b1, b2, b3, b4; /* for calculating psi */
    double c1, c2, c3, c4, c5, c6, c7, c8, c9, c10; /* for calculating zeta */
};

/* Simulation of 2D coordination */
class cylinderCoordinate{
private:
    int leftboundary, rightboundary, upboundary, downboundary;
    cylinderNode* coordinate; /* array storing nodes */
public:
    cylinderCoordinate(int l, int r, int u, int d){
        leftboundary = l;
        rightboundary = r;
        upboundary = u;
        downboundary = d;
        coordinate = new cylinderNode[(r - l + 1) * (u - d + 1)];
    }
    ~cylinderCoordinate(){
        delete [] coordinate;
    }
    cylinderNode& access(int x, int y){
        return coordinate[x - l + (y - d) * (r - l + 1)];
    }
};
        

class cylinderProject{
private:
    double t; /* Physical time */
    cylinderCoordinate* coordination; /* transformed coordination */
    int leftboundary, rightboundary, upboundary, downboundary;
protected:
    const double deltaxi; /* delta xi and delta eta */
    const double deltaeta; /* delta xi and delta eta */
    const double deltat; /* time step */
    const double Re; /* 2 * physical Reynolds */
public:
    cylinderProject(int l, int r, int u, int d, double dxi = 0.1, double deta = 0.1, double dt = 0.1, double rey = 40):deltaxi(dxi), deltaeta(deta), deltat(dt), leftboundary(l), rightboundary(r), upboundary(u), downboundary(d){
        t = 0;
        Re = 2 * rey;
        coordination = new cylinderCoordinate(l, r, u, d);
    }
    ~cylinderProject(){
        delete coordination;
    }

    void initialize();
    void run();
};
    

#endif /* _cylinder */

/* radius = 1 */
