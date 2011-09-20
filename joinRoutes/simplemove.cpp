#include "simplemove.h"

SimpleMove::SimpleMove(int indexPoint, int numRoutePoint, int indexPointSucc, int numRouteSucc)
{
    this->indexPoint = indexPoint;
    this->numRoutePoint = numRoutePoint;
    this->indexPointSucc = indexPointSucc;
    this->numRouteSucc = numRouteSucc;
}

int SimpleMove::getIndexPoint(){
    return this->indexPoint;
}

int SimpleMove::getNumRoutePoint(){
    return this->numRoutePoint;
}

int SimpleMove::getIndexPointSucc(){
    return this->indexPointSucc;
}

int SimpleMove::getNumRouteSucc(){
    return this->numRouteSucc;
}
