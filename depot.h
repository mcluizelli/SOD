/*
###################################################################################################################
    This class represent the depot in SOD. A set of orders are assign to depot. The set of orders are routed and
    storaged in lists of indexes, which represent orders.
    @author Marcelo
    @date   27/07/2011
###################################################################################################################
*/

#ifndef DEPOT_H
#define DEPOT_H
#include <QList>
#include "route.h"

class SOD;

class Depot
{

private:
    //This index represent such that in the global list of depots.
    int      iIndexDepot;
    //This index represent such that in the global list of orders.
    int      iIndexOder;
    int      iType;
    long int iCoordX;
    long int iCoordY;
    float    nCapacity;
    float    nRouteTime;
    QList<int> ordersAlocated;
    QList<Route*> routes;

public:

    Depot();
    Depot(int iIndex, int iIndexOrder,long int iCoordX, long int iCoordY, int iType, float nCapacity);
    QList<Route*> getRoutes();
    QList<int> getOrdersAlocated();
    Route* getRoute(int iIndexRoute);
    void addOrder(int i);
    void addRoute(Route *oRoute);
    void removeRoute(int iIndexRoute);
    long int getCoordX() const;
    long int getCoordY() const;
    int getType();
    int getNumRoutes();
    int getRouteByOrder(int iIndexOrder);
    int getIndexDepot();
    int getIndexOfOrder();
    int getNumOrdersAlocated();
    float getCapacity();
    float getDemandByRoute(int iRoute);
};

#endif // DEPOT_H
