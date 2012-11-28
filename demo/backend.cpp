#include <cmath>
#include "backend.h"

const double pendulum::gravity = 9.8;

void pendulum::run()
{
    double acceleration = -gravity*sin(theta);
    theta += (velocity * step + acceleration * step * step/2)/length;
    velocity += acceleration * step;
}

