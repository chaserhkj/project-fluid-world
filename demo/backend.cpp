#include <cmath>
#include "backend.h"


void pendulum::run()
{
    double acceleration = -gravity*sin(theta);
    theta += (v * step + acceleration * step * step/2)/length;
    velocity += acceleration * step;
}

