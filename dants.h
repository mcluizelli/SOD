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


class DAnts
{

private:

    int          iNumMaxIterationDAnts;
    int          iNs;
    float        nCostBestSolution;
    SOD          oSodInitialSolution;
    SOD          *oBestSolution;
    PheromoneInf *oPheromoneInf;

    QList<RouteCenterGravity> calculeCenterGravityToRoutes(Depot *oDepot);
    QList<SOD*> sweepAlgorithmModified(QList<RouteCenterGravity> oCentersGravity, SOD *oCurrentSolution, int iContDepot);
    void applySbASCluster(int iDepot, SOD* oSolution, QList<SOD*> oListSubProblems);
public:

    DAnts(SOD oSodInitialSolution, int iNumMaxIterationDAnts);
    SOD* run();

};

#endif // DANTS_H
