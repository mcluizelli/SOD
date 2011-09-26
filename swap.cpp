#include "swap.h"

Swap::Swap(SOD *opSolution)
{
    this->opSolution = opSolution;
}

void Swap::performeMove(){

    int iNumDepot = this->opSolution->getNumberDepot();
    float nCostRouteAux = 0.0, nNewCostRouteAux = 0.0;
    bool hasImprove = true;
    int iIndexOrderBest1 = 0, iIndexOrderBest2 = 0;

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
                    QList<OrderRoute> oListNeighbor = opDepot->getNeighbor(oRoute1->at(iContOrder), iContRoute1);

                    for(int iContNeighbor = 0; iContNeighbor < oListNeighbor.size(); iContNeighbor++){

                        //Here its try swap the points.
                        //Evaluetes the restrictions.

                        //Valores
                        int iStoreValueIndex         = oRoute1->at(iContOrder);
                        int iStoreValueIndexNeighbor = oListNeighbor.at(iContNeighbor).iValue;

                        //Indices
                        int iIndex  = iContOrder;
                        int iIndex2 = oListNeighbor.at(iContNeighbor).iIndexOrder;

                        //Order
                        Order *oOrder1 = this->opSolution->getOrder(iIndex);
                        Order *oOrder2 = this->opSolution->getOrder(iIndex2);

                        //Routes
                        int iRoute  = iContRoute1;
                        int iRoute2  = oListNeighbor.at(iContNeighbor).iIndexRoute;

                        float nCapacityRoute = this->opSolution->getDemandRouteByDepotIndex(iContDepot, iRoute2);
                        float nCapacityRoute2 = this->opSolution->getDemandRouteByDepotIndex(iContDepot, iRoute);

                        if( (nCapacityRoute + oOrder1->getDemand() <= opDepot->getCapacity())
                                && (nCapacityRoute2 + oOrder2->getDemand() <= opDepot->getCapacity()) ){
                            //Try move;

                            nNewCostRouteAux = nCostRouteAux = this->opSolution->getCostRouteByDepot(opDepot->getIndexDepot(), iContRoute1);

                            if(nNewCostRouteAux < nCostRouteAux){

                                iIndexOrderBest1 = iContOrder;
                                iIndexOrderBest2 = oListNeighbor.at(iContNeighbor);

                            }

                        }

                    }

                }

            }

            if(hasImprove){

            }


        }


    }
}
