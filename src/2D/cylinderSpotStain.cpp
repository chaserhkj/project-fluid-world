#include <cmath>
#include "cylinder.h"

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
    
    calculateXY();
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
        spot->calculateXY();
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

