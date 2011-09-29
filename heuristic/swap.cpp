#include <QDebug>
#include "swap.h"

Swap::Swap(SOD *opSolution)
{
    this->opSolution = opSolution;
}

void Swap::performeMove(){

    float nBetterDiference = this->opSolution->getCostSolution();
    int iNumDepot = this->opSolution->getNumberDepot();
    float nCostRouteAux = 0.0, nNewCostRouteAux = 0.0;
    bool hasImprove = true;
    int iRouteBest1 = 0, iRouteBest2 = 0;
    int iIndexBest1 = 0, iIndexBest2 = 0;
    int iValueBest1 = 0, iValueBest2 = 0;

    for(int iContDepot = 0; iContDepot < iNumDepot; iContDepot++){

        Depot *opDepot = this->opSolution->getDepot(iContDepot);
        QList<Route*> oRoutes = opDepot->getRoutes();

        while(hasImprove){

            hasImprove = false;

            for(int iContRoute1 = 0; iContRoute1 < oRoutes.size(); iContRoute1++){

                QList<int> *oRoute1 = oRoutes.at(iContRoute1)->getRoute();
                //Get cost of route.
                nCostRouteAux = this->opSolution->getCostRouteByDepot(opDepot->getIndexDepot(), iContRoute1);

                for( int iContOrder = 0; iContOrder < oRoute1->size(); iContOrder++){

                    //OrderRoute store index of route and index of order.
                    QList<OrderRoute> oListNeighbor = this->opSolution->getNeighbor(iContDepot, oRoute1->at(iContOrder), iContRoute1);

                    for(int iContNeighbor = 0; iContNeighbor < oListNeighbor.size(); iContNeighbor++){

                        //Here its try swap the points.
                        //Evaluetes the restrictions.

                        //Valores
                        int iStoreValueIndex         = oRoute1->at(iContOrder);
                        int iStoreValueIndexNeighbor = oListNeighbor.at(iContNeighbor).iValue;

                        //Indices
                        int iIndex  = oRoute1->at(iContOrder);
                        int iIndex2 = oListNeighbor.at(iContNeighbor).iIndexOrder;

                        //Order
                        Order *oOrder1 = this->opSolution->getOrder(iIndex);
                        Order *oOrder2 = this->opSolution->getOrder(iIndex2);

                        //Routes
                        int iRoute  = iContRoute1;
                        int iRoute2  = oListNeighbor.at(iContNeighbor).iIndexRoute;

                        float nCapacityRoute  = this->opSolution->getDemandRouteByDepotIndex(iContDepot, iRoute2);
                        float nCapacityRoute2 = this->opSolution->getDemandRouteByDepotIndex(iContDepot, iRoute);

                        if( (nCapacityRoute - oOrder2->getDemand() + oOrder1->getDemand() <= opDepot->getCapacity())
                                && (nCapacityRoute2 - oOrder1->getDemand() + oOrder2->getDemand() <= opDepot->getCapacity()) ){

                            //Try move;

                            oRoute1->replace(iContOrder, iStoreValueIndexNeighbor);
                            Route *oRoute2 = opDepot->getRoute(iRoute2);
                            oRoute2->getRoute()->replace(iIndex2, iStoreValueIndex);

                            //nNewCostRouteAux = this->opSolution->getCostRouteByDepot(opDepot->getIndexDepot(), iContRoute1);
                            nNewCostRouteAux = this->opSolution->getCostSolution();

                            if(nNewCostRouteAux < nBetterDiference){
                                nBetterDiference = nNewCostRouteAux;
                                //Store route and order index.
                                iIndexBest1 = iContOrder;
                                iIndexBest2 = iIndex2;
                                iRouteBest1 = iContRoute1;
                                iRouteBest2 = iRoute2;
                                iValueBest1 = iStoreValueIndex;
                                iValueBest2 = iStoreValueIndexNeighbor;
                                hasImprove  = true;

                            }

                            oRoute1->replace(iContOrder, iStoreValueIndex);
                            oRoute2->getRoute()->replace(iIndex2, iStoreValueIndexNeighbor);


                        }

                    }

                }

            }

            if(hasImprove){

                Route *oRoute1 = opDepot->getRoute(iRouteBest1);
                Route *oRoute2 = opDepot->getRoute(iRouteBest2);

                oRoute1->getRoute()->replace(iIndexBest1, iValueBest2);
                oRoute2->getRoute()->replace(iIndexBest2, iValueBest1);
                hasImprove = true;

            }


        }


    }
}
