#include "order.h"

Order::Order()
{
}

Order::Order(int iIndex, long int iCoordX, long int iCoordY, float nDemand, int iPriority){

    this->iIndex = iIndex;
    this->iCoordX = iCoordX;
    this->iCoordY = iCoordY;
    this->nDemand = nDemand;
    this->iPriority = iPriority;

}

float Order::getDemand() const{
    return this->nDemand;
}

long int Order::getCoordX() const{
    return this->iCoordX;
}

long int Order::getCoordY() const{
    return this->iCoordY;
}

int Order::getIndex() const{
    return this->iIndex;
}

int Order::getPriority() const{
    return this->iPriority;
}

void Order::setDemand(float nDemand){
    this->nDemand = nDemand;
}

void Order::setCoordX(long int iCoordX){
    this->iCoordX = iCoordX;
}

void Order::setCoordY(long int iCoordY){
    this->iCoordY = iCoordY;
}

void Order::setPriority(int iPriority){
    this->iPriority = iPriority;
}

void Order::setIndex(int iIndex){
    this->iIndex = iIndex;
}
