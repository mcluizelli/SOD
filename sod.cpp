#include "sod.h"
#include <QDebug>
#include <iostream>
#include <math.h>

SOD::SOD()
{
}

SOD::~SOD()
{

   /* foreach(Order *p, listOrder){
       free(p);
    }

    foreach(Depot *p, listDepot){
       free(p);
    }
    */
}

/**
  *Return a Order that represent the cetner of gravity of a route.
  *@param Route* oRoute
  *@return Order
  *@author Marcelo
**/
Order SOD::calculeCenterGravity(Route *oRoute){

    Order oCenter;
    QList<int> *oListOrder = oRoute->getRoute();
    float nSumX = 0.0, nSumY = 0.0, nSumDemand = 0.0;
    float nDemandAux = 0.0;

    for(int iContOrder = 0; iContOrder < oListOrder->size(); iContOrder++){
        nDemandAux =  this->listOrder.at(oListOrder->at(iContOrder))->getDemand();
        nSumDemand += nDemandAux;
        nSumX      += this->listOrder.at(oListOrder->at(iContOrder))->getCoordX() * nDemandAux;
        nSumY      += this->listOrder.at(oListOrder->at(iContOrder))->getCoordY() * nDemandAux;
    }

    nSumX = nSumX/nSumDemand;
    nSumY = nSumY/nSumDemand;

    oCenter.setCoordX((long)nSumX);
    oCenter.setCoordY((long)nSumY);

    return oCenter;

}

/**
  *Return a specified order by index.
  *@param int
  *@return Order
  *@author Marcelo
**/
Order* SOD::getOrder(int iIndex) const{

    Order *opOrderAux;

    try{
        opOrderAux = listOrder.at(iIndex);
    }catch(...){
        qDebug() << "Wrong to return order by index. Index out of range.";
        return NULL;
    }

    return opOrderAux;

}

/**
  *Return total of demand acumulated on route.
  *@param Depot, int
  *@return float
  *@author Marcelo
**/
float  SOD::getDemandRoute(Route *oRoute){

    float nDemand = 0.0;
    QList<int> *oListOrders = oRoute->getRoute();
    for(int iCont = 0; iCont < oListOrders->size(); iCont++){
        nDemand += this->listOrder.at(oListOrders->at(iCont))->getDemand();
    }
    return nDemand;
}

/**
  *Return a specified depot by index.
  *@param int
  *@return Depot
  *@author Marcelo
**/
Depot* SOD::getDepot(int iIndex){
/*
    Depot *opDepotAux;
    try{
        opDepotAux = listDepot.at(iIndex);
    }catch(...){
        qDebug() << "Wrong to return depot by index. Index out of range.";
        return NULL;
    }
    return opDepotAux;
*/
    return listDepot.at(iIndex);
}

/**
  *Set the number of depot.
  *@param int
  *@return void
  *@author Marcelo
**/
void SOD::setNumberDepot(int iNumberDepot){
    this->iNumberDepot = iNumberDepot;
}

/**
  *Return the distance between two Orders represented by your index.
  *@param int, int
  *@return float
  *@author Marcelo
**/
float SOD::getDistance(int iIndex1, int iIndex2){

    Order *oOrder1 = getOrder(iIndex1);
    Order *oOrder2 = getOrder(iIndex2);

    return sqrt( pow(oOrder1->getCoordX() - oOrder2->getCoordX(), 2) + pow(oOrder1->getCoordY() - oOrder2->getCoordY(), 2) );
}

/**
  *Return the number of depots.
  *@param void
  *@return int
  *@author Marcelo
**/
int SOD::getNumberDepot(){
    return this->iNumberDepot;
}

/**
  *Adicione order to solution.
  *@param iIndex, iCoordX, iCoordY, nDemand, iPriority
  *@return void
  *@author Marcelo
**/
void SOD::addOrder(int iIndex, long int iCoordX, long int iCoordY, float nDemand, int iPriority){
    Order *order = new Order(iIndex, iCoordX, iCoordY, nDemand, iPriority);
    listOrder.append(order);
}

void SOD::addDepot(int iIndexDepot, int iIndexOrder, long int iCoordX, long int iCoordY, int iType, float nCapacity){
    Depot *opDepot = new Depot(iIndexDepot, iIndexOrder, iCoordX, iCoordY, iType, nCapacity);
    this->listDepot.append(opDepot);
}

/**
  *Show orders.
  *@param void
  *@return void
  *@author Marcelo
**/
void SOD::showOrders(){

    foreach( Order *p, listOrder){
        qDebug() << "(" << p->getCoordX() << "," << p->getCoordY() << ")" << " - " << p->getDemand();
    }

}
/**
  *Return copy of SOD solution. This copy is based on list of orders passed as param. This copy is used in D-Ants algorithm.
  *@param QList<int> oListOrders, Depot* oDepot
  *@return SOD*
  *@author Marcelo
**/
SOD* SOD::copy(QList<int> *oListOrders, Depot *oDepot){

    SOD *oCopy = new SOD;
    oCopy->setNumberDepot(1);

    //Do copy of orders.
    for(int iCont = 0; iCont < this->listOrder.size(); iCont++){
        Order *p = this->listOrder.at(iCont);
        oCopy->addOrder(p->getIndex(), p->getCoordX(), p->getCoordY(), p->getDemand(), p->getPriority());
    }

    //Do copy of depots.
    Depot *d = listDepot.at(oDepot->getIndexDepot());
    oCopy->addDepot(0, d->getIndexOfOrder(), d->getCoordX(), d->getCoordY(), d->getType(), d->getCapacity());
    Depot* oDepotAux = oCopy->getDepot(d->getIndexDepot());

    for(int iOrder = 0; iOrder < oListOrders->size(); iOrder++){
        oDepotAux->addOrder(oListOrders->at(iOrder));
    }

    //return copy.
    return oCopy;
}

/**
  *Return full copy of SOD solution.
  *@param void
  *@return SOD*
  *@author Marcelo
**/
SOD* SOD::copy(){

    SOD *oCopy = new SOD;
    oCopy->setNumberDepot(this->iNumberDepot);

    //Do copy of orders.
    foreach(Order *p, this->listOrder){
        oCopy->addOrder(p->getIndex(), p->getCoordX(), p->getCoordY(), p->getDemand(), p->getPriority());
    }

    //Do copy of depots.
    foreach(Depot *d, this->listDepot){

        oCopy->addDepot(d->getIndexDepot(), d->getIndexOfOrder(), d->getCoordX(), d->getCoordY(), d->getType(), d->getCapacity());
        Depot* oDepotAux = oCopy->getDepot(d->getIndexDepot());
        QList<int> oListOrdersAlocated = d->getOrdersAlocated();

        foreach(int iOrder, oListOrdersAlocated){
            oDepotAux->addOrder(iOrder);
        }

    }

    //return copy.
    return oCopy;

}

/**
  *Return number of orders in solution.
  *@param void
  *@return int
  *@author Marcelo
**/
int SOD::getNumOrders(){
    return listOrder.size();
}

/**
  *Show on screem the depots and your assigned orders.
  *@param void
  *@return void
  *@author Marcelo
**/
void SOD::showDepots(){
    int iCont = 0;
    qDebug() << "List of depots";
    foreach(Depot *opDepotAux, listDepot){
        qDebug() << iCont << " - "<< opDepotAux->getCoordX() << "," << opDepotAux->getCoordY();
        qDebug() << "List of orders" << opDepotAux->getOrdersAlocated().size();
        foreach( int iOrder, opDepotAux->getOrdersAlocated()){
            qDebug() << iOrder;
        }

        iCont++;
    }

}


/**
  *Show Solution.
  *@param void
  *@return void
  *@author Marcelo
**/
void SOD::showSolution(){

    foreach( Depot *depotAux, listDepot){
        foreach( Route *routeAux, depotAux->getRoutes()){
            routeAux->showRoute();
        }
    }

}

/**
  *This method is responsable to alocate orders to depots. Many cluster algorithm.In this case, one depot is presuposed.
  *@param void
  *@return void
  *@author Marcelo
**/
void SOD::alocateOrderToDepots(){
    //Create depot and alocate.
    Depot *opNewDepot = listDepot.at(0);
    for( int iCont = 0; iCont < this->listOrder.size(); iCont++){
        if(listOrder.at(iCont)->getDemand() != -1) opNewDepot->addOrder(listOrder.at(iCont)->getIndex());
    }
}

/**
  *This method calculates the total cost of solutions.
  *@param void
  *@return float
  *@author Marcelo
**/
float SOD::getCostSolution(){

    float nCost = 0.0;

    for(int iCont = 0; iCont < listDepot.size(); iCont++){
        //For each depot is calculated the total cost.
        nCost += getCostDepot(iCont);
    }

    return nCost;

}

QList<OrderRoute> SOD::getNeighbor(int iDepot, int iIndexOrder, int iIndexRoute){

    float nDistMax = 50, nDist = 0.0;
    QList<OrderRoute> oListNeighbor;
    Depot *oDepot = listDepot.at(iDepot);
    QList<Route*> oListRoute = oDepot->getRoutes();
    oListNeighbor.clear();

    for(int iContRoute = 0; iContRoute < oListRoute.size(); iContRoute++){

        if(iContRoute != iIndexRoute){
            Route *oRoute = oListRoute.at(iContRoute);
            QList<int> *oOrders = oRoute->getRoute();
            for(int iContOrder = 0; iContOrder < oOrders->size(); iContOrder++){

                nDist = getDistance(iIndexOrder, oOrders->at(iContOrder));
                if(nDist <= nDistMax){
                    OrderRoute r;
                    r.iIndexOrder = iContOrder;
                    r.iIndexRoute = iContRoute;
                    r.iValue      = oOrders->at(iContOrder);
                    oListNeighbor.append(r);
                }

            }

        }

    }

    return oListNeighbor;

}

float  SOD::getDemandRouteByDepotIndex(int iIndexDepot, int iIndexRoute){

    float nCost = 0.0;

    Depot *opDepotAux = listDepot.at(iIndexDepot);
    QList<Route*> oListRoutes = opDepotAux->getRoutes();

    Route *oRouteAux = oListRoutes.at(iIndexRoute);
    QList<int> *oListOrdersRoute = oRouteAux->getRoute();


    for(int iCont3 = 0; iCont3 < oRouteAux->getRoute()->size() ; iCont3++){
        nCost += this->listOrder.at(oListOrdersRoute->at(iCont3))->getDemand();
    }


    return nCost;

}


float SOD::getCostRouteByDepot(int iIndexDepot, int iIndexRoute){

    float nCost = 0.0;

    Depot *opDepotAux = listDepot.at(iIndexDepot);
    QList<Route*> oListRoutes = opDepotAux->getRoutes();

    Route *oRouteAux = oListRoutes.at(iIndexRoute);
    QList<int> *oListOrdersRoute = oRouteAux->getRoute();

    nCost += getDistance(opDepotAux->getIndexOfOrder(), oListOrdersRoute->at(0));

    for(int iCont3 = 0; iCont3 < oRouteAux->getRoute()->size() -1 ; iCont3++){
        nCost += getDistance(oListOrdersRoute->at(iCont3), oListOrdersRoute->at(iCont3 + 1));
    }

     nCost += getDistance(opDepotAux->getIndexOfOrder(), oListOrdersRoute->at(oListOrdersRoute->size() - 1));

    return nCost;

}

/**
  *This method calculates the cost associeate to specific depot.
  *@param int
  *@return float
  *@author Marcelo
**/
float SOD::getCostDepot(int iIndexDepot){

    float nCost = 0.0;
    Depot *opDepotAux = listDepot.at(iIndexDepot);
    QList<Route*> oListRoutes = opDepotAux->getRoutes();

    for(int iCont2 = 0; iCont2 < oListRoutes.size(); iCont2++){

        Route *oRouteAux = oListRoutes.at(iCont2);
        QList<int> *oListOrdersRoute = oRouteAux->getRoute();

        nCost += getDistance(opDepotAux->getIndexOfOrder(), oListOrdersRoute->at(0));

        for(int iCont3 = 0; iCont3 < oRouteAux->getRoute()->size() -1 ; iCont3++){
            nCost += getDistance(oListOrdersRoute->at(iCont3), oListOrdersRoute->at(iCont3 + 1));
        }

        nCost += getDistance(opDepotAux->getIndexOfOrder(), oListOrdersRoute->at(oListOrdersRoute->size() - 1));

    }

    return nCost;

}
