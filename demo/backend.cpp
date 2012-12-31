#include <cmath>
#include "backend.h"

const double pendulum::gravity = 9.8;

void pendulum::run()
{
    double acceleration, deltaTheta;
    acceleration = -gravity * sin(theta);
    int i;

    for (i = 0; i < recurrenceDepth; i++) {
        deltaTheta = (velocity * step + acceleration * step * step / 2) / length;
        acceleration = (acceleration + -gravity * sin(theta + deltaTheta)) / 2;
    }

    velocity += acceleration * step;
    theta += deltaTheta;
    time += step;
}
