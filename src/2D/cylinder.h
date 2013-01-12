#ifndef _cylinder
#define _cylinder

#include <interface.h>
#include "commontypes.h"

class FWDataVarient
{
private:
    DATA type;
    cylinderNode* node;
    cylinderSpotStain* spot;
public:
    FWDataVarient(DATA t, cylinderNode* n):type(t), node(n){};
    FWDataVarient(DATA t, cylinderSpotStain* s):type(t), spot(s){};

    friend class cylinderProject;
}

class cylinderProject;

class cylinderTransformer: public Transformer
{
public:
    void calculateXY();
};

class cylinderNode: public Node, public cylinderTransformer
{
public:
    double hxi;           /* lame coefficient */
    double heta;
    double b0, b1, b2, b3, b4;    /* for calculating psi */
    double c1, c2, c3, c4, c5, c6, c7, c8, c9, c10;   /* for calculating zeta */
    double uxi, ueta;     /* velocity in the transformed coordination */
};

class cylinderSpotStain: public cylinderTransformer
{
private:
    cylinderProject * project;    /* To know if I am out of range */
public:
    cylinderSpotStain * next;

    cylinderSpotStain(cylinderProject * p): project(p) {
        next = NULL;
    }

    void destroychain();

    bool isOutofRange();
    void run();
};

class cylinderSpotStainSource
{
private:
    int step;         /* distance between two nearby sources */
    int number;           /* number of sources */
    cylinderSpotStain ** source;  /* source array */
    cylinderProject * project;
public:
    cylinderSpotStainSource(cylinderProject * p);

    ~cylinderSpotStainSource() {
        int i;

        for (i = 0; i < number; i++) {
            if (source[i]) {
                source[i]->destroychain();
            }
        }

        delete[] source;
    }

    void run();

    int getNumber(){
        return number;
    }

    cylinderSpotStain * operator[](int i) {
        return source[i];
    }
};


/* Simulation of 2D coordination */
class cylinderCoordinate
{
private:
    int leftboundary, rightboundary, upboundary, downboundary;
    cylinderNode * coordinate; /* array storing nodes */
public:
    cylinderCoordinate(int l, int r, int u, int d) {
        leftboundary = l;
        rightboundary = r;
        upboundary = u;
        downboundary = d;
        coordinate = new cylinderNode[(r - l + 1) * (u - d + 1)];
    }
    ~cylinderCoordinate() {
        delete[]coordinate;
    }
    cylinderNode & access(int x, int y) {
        return coordinate[x - leftboundary +
                          (y - downboundary) * (rightboundary - leftboundary + 1)];
    }
};

class cylinderProject:public Project
{
private:
    double t;         /* Physical time */
    double Re;            /* 2 * physical Reynolds */
    cylinderCoordinate * coordination; /* transformed coordination */
    cylinderCoordinate * cylinderBoundary; /* Boundary nodes on the cylinder, 1 means upper half and 0 means lower half */
    int leftboundary, rightboundary, upboundary, downboundary;
    int leftterminal, rightterminal;  /* Terminals on the cylinder */
    double density;       /* spotstain density, from 0 to 1 */
    cylinderSpotStainSource * source; /* source of spotstains */

    /* convert 2-D points to 1-D for matrix solver */
    int psiConvert(int i, int j);
    int zetaConvert(int i, int j);

    void initialize();
    void calculateBoundaryZeta();
    void calculateNewZeta();
    void timeStep();
    void calculateNewPsi();
    void calculateVelocity();

    bool OnBoundary(int i, int j);    
protected:
    const double deltaxi;     /* delta xi and delta eta */
    const double deltaeta;    /* delta xi and delta eta */
    const double deltat;      /* time step */
public:
    cylinderProject(int l = -100, int r = 400, int u = 100, int d = -100, double dens = 0.1, double dxi = 0.1, double deta = 0.1, double dt = 0.1, double rey = 40): leftboundary(l), rightboundary(r), upboundary(u), downboundary(d),
        density(dens), deltaxi(dxi), deltaeta(deta), deltat(dt)
         {
        t = 0;
        Re = 2 * rey;
        leftterminal = -1 / deltaxi;
        rightterminal = 1 / deltaxi;
        coordination = new cylinderCoordinate(l, r, u, d);
        cylinderBoundary = new cylinderCoordinate(-1 / deltaxi, 1 / deltaxi, 1, 0);
        source = new cylinderSpotStainSource(this);
    }
    ~cylinderProject() {
        delete coordination;
        delete cylinderBoundary;
        delete source;
    }

    /*double getT() {
        return t;
    }
    cylinderSpotStainSource* getSource() {
        return source;
    }
    cylinderCoordinate * getCoordinate() {
        return coordination;
    }*/

    int getSourceNumber(){
        return source->getNumber();
    }
    
    double getData(DATA data);
    FWDataVarient getData(DATA data, double xip, double etap);
    FWDataVarient getData(DATA data, int n);

    double getX(FWDataVarient& data);
    double getY(FWDataVarient& data);
    double getPsi(FWDataVarient& data);
    bool next(FWDataVarient& data);
    

    void setDensity(double dens) {
        density = dens;
        delete source;
        source = new cylinderSpotStainSource(this);
    }

    friend class cylinderSpotStain;
    friend class cylinderSpotStainSource;

    void run();
    void spotstainrun();
};

#endif /* _cylinder */

/* radius = 1 */
