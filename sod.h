/*
###################################################################################################################
    This class represent a solution of service of dispachter problem.
    @author Marcelo
    @date   27/07/2011
###################################################################################################################
*/
#ifndef SOD_H
#define SOD_H

#include <QList>
#include "order.h"
#include "depot.h"

class SOD
{

private:

    int iNumberDepot;
    //listOrder storages the global list of order.
    QList<Order*> listOrder;
    //listDepot storages de complete list of depots. Each depot storages a route lists.
    QList<Depot*> listDepot;


public:

    SOD();
    ~SOD();
    Order* getOrder(int iIndex) const;
    Order  calculeCenterGravity(Route *oRoute);
    Depot* getDepot(int iIndex);
    float  getDistance(int iIndex1, int iIndex2);
    float  getCostSolution();
    float  getCostDepot(int iIndexDepot);
    float  getDemandRoute(Route *oRoute);
    void   addOrder(int iIndex, long int iCoordX, long int iCoordY, float nDemand, int iPriority);
    void   addDepot(int iIndexDepot, int iIndexOrder, long int iCoordX, long int iCoordY, int iType, float nCapacity);
    void   showOrders();
    void   showDepots();
    void   showSolution();
    void   alocateOrderToDepots();
    void   setNumberDepot(int iNumberDepot);
    int    getNumberDepot();
    int    getNumOrders();
    SOD*   copy();
    SOD*   copy(QList<int> oListOrders, Depot *oDepot);
};

#endif // SOD_H
