#ifndef _backend
#define _backend

enum Colour {RED, YELLOW, PINK, GREEN, PURPLE, ORANGE, BLUE};

struct pressureNode{
    double p;
    bool isBoundary;
}

struct velocityNode{
    double vx; /*Velocity along the X-axis*/
    double vy;
    double vz;
    bool isBoundary;
}

class paintedPoint{
private:
    double x; /*X-axis*/
    double y;
    double z;
    double vx; /*Velocity along the X-axis*/
    double vy;
    double vz;
    Colour c;
protected:
    double step; /*Time step*/
public:
    void calculateVelocity();
    paintedPoint(double xp, double yp, double zp, Colour color) : x(xp), y(yp), z(zp), c(color) {
        this->calculateVelocity();
    }
    void run();
}

#endif /*_backend*/
