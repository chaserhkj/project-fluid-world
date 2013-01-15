#include "cylinder.h"

double cylinderDataVariant::getX()
{
    if (type == Project::PsiType){
        return ((cylinderNode*)data)->x;
    } else {
        return ((cylinderSpotStain*)data)->x;
    }
}

double cylinderDataVariant::getY()
{
    if (type == Project::PsiType){
        return ((cylinderNode*)data)->y;
    } else {
        return ((cylinderSpotStain*)data)->y;
    }
}

double cylinderDataVariant::getZ()
{
    /* in 2D-plate */
    return 0;
}

double cylinderDataVariant::getPsi()
{
    return ((cylinderNode*)data)->psi;
}

double cylinderDataVariant::getTime()
{
    return *((double*)data);
}

bool cylinderDataVariant::next()
{
    cylinderSpotStain* spot = (cylinderSpotStain*)data;
    if (spot->next){
        data = spot->next;
        return true;
    } else {
        return false;
    }
}
        
