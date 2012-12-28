#ifndef _cylinder
#define _cylinder

#include "commontypes.h"

class cylinderNode : public Node {};

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
}
        

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
    cylinderProject(int l, int r, int u, int d, double dxi = 0.1, double deta = 0.1, double dt = 0.1, double rey):deltaxi(dxi), deltaeta(deta), deltat(dt), leftboundary(l), rightboundary(r), upboundary(u), downboundary(d){
        t = 0;
        Re = 2 * rey;
        coordination = new cylinderCoordinate(l, r, u, d);
    }
    ~cylinderProject(){
        delete coordination;
    }

    void initialize();
    void run();
}
    

#endif /* _cylinder */

/* radius = 1 */
