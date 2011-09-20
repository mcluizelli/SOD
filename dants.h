#ifndef DANTS_H
#define DANTS_H
#include "sod.h"
#include "pheromoneinf.h"
#include "route.h"
#include "depot.h"
#include "order.h"

class RouteCenterGravity{

public:

    Route    *oRoute;
    //Order that represent the center of gravity.
    Order    oCenterGravity;
    //Number of subproblens.

};

class RouteIndexByCluster{

public:
    //Store the index of routes
    QList<int> oListIndexRoutes;

};


class DAnts
{

private:

    //private atribute.
    int                         iNumMaxIterationDAnts;
    int                         iNs;
    float                       nCostBestSolution;
    SOD                         oSodInitialSolution;
    SOD                         *oBestSolution;
    PheromoneInf                *oPheromoneInf;
    QList<RouteIndexByCluster>  oRouteIndexByCluster;

    //private methods.
    QList<RouteCenterGravity>   calculeCenterGravityToRoutes(Depot *oDepot);
    QList<SOD*>                 sweepAlgorithmModified(QList<RouteCenterGravity> oCentersGravity, SOD *oCurrentSolution, int iContDepot);
    QList<SOD*>                 applySbASCluster(QList<SOD*> oListSubProblems);
    void                        compareResults(int iDepot, SOD *oSolution, QList<SOD*> oListSubProblems);

public:

    DAnts(SOD oSodInitialSolution, int iNumMaxIterationDAnts);
    SOD* run();

};

#endif // DANTS_H
