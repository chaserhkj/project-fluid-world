#include <iostream>
#include <cmath>

const double step = 0.01;
const double gravity = 9.8;
double theta;
double velocity;
double length;
double time;

void pendulum(void)
{
    double acceleration = -gravity*sin(theta);
    theta += (v * step + acceleration * step * step/2)/length;
    v += acceleration * step;
    time += step;
}

