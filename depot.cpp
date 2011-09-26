#include "depot.h"

Depot::Depot()
{
}

Depot::Depot(int iIndexDepot, int iIndexOrder,long int iCoordX, long int iCoordY, int iType, float nCapacity)
{

    this->iIndexDepot = iIndexDepot;
    this->iIndexOder  = iIndexOrder;
    this->iCoordX     = iCoordX;
    this->iCoordY     = iCoordY;
    this->iType       = iType;
    this->nCapacity   = nCapacity;
    this->nRouteTime  = 0.0;

}



/**
  *Return all routes.
  *@param void
  *@return QList<Route>
  *@author Marcelo
**/
QList<Route*> Depot::getRoutes(){
    return this->routes;
}

/**
  *Return specific route defined by index.
  *@param int
  *@return Route
  *@author Marcelo
**/
Route* Depot::getRoute(int iIndexRoute){
    return this->routes.at(iIndexRoute);
}

void Depot::addOrder(int iOrder){
    this->ordersAlocated.append(iOrder);
}

void Depot::addRoute(Route *oRoute){
    this->routes.append(oRoute);
}

void Depot::removeRoute(int iIndexRoute){
    this->routes.removeAt(iIndexRoute);
}

QList<int> Depot::getOrdersAlocated() {
    return this->ordersAlocated;
}

QList<OrderRoute> Depot::getNeighbor(int iIndexOrder, int iIndexRoute){

}

long int Depot::getCoordX() const{
    return this->iCoordX;
}

long int Depot::getCoordY() const{
    return this->iCoordY;
}

int Depot::getType(){
    return this->iType;
}

int Depot::getNumOrdersAlocated(){
    return this->ordersAlocated.size();
}

int Depot::getNumRoutes(){
    return this->routes.size();
}

/**
  *Return number route that contain specifc order.
  *@param int
  *@return int
  *@author Marcelo
**/
int Depot::getRouteByOrder(int iIndexOrder){

    for(int iCont = 0; iCont < this->routes.size(); iCont++){

        QList<int> *oRouteAux = this->routes.at(iCont)->getRoute();
        for(int iCont2 = 0; iCont2 < oRouteAux->size(); iCont2++){

            if(oRouteAux->at(iCont2) == iIndexOrder){
                return iCont;
            }

        }
    }

    return -1;

}

int Depot::getIndexDepot(){
    return this->iIndexDepot;
}

int Depot::getIndexOfOrder(){
    return this->iIndexOder;
}

float Depot::getCapacity(){
    return this->nCapacity;
}


