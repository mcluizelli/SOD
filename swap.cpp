#include "swap.h"

Swap::Swap(SOD *opSolution)
{
    this->opSolution = opSolution;
}

void Swap::performeMove(){

    int iNumDepot = this->opSolution->getNumberDepot();
    bool hasImprove = true;

    for(int iContDepot = 0; iContDepot < iNumDepot; iContDepot++){

        Depot *opDepot = this->opSolution->getDepot(iContDepot);
        QList<Route*> oRoutes = opDepot->getRoutes();
        while(hasImprove){

            hasImprove = false;

            for(int iContRoute1 = 0; iContRoute1 < oRoutes.size(); iContRoute1++){

                QList<int> *oRoute1 = oRoutes.at(iContRoute1)->getRoute();
                for( int iContOrder = 0; iContOrder < oRoute1->size(); iContOrder++){

                    QList<OrderRoute> oListNeighbor = opDepot->getNeighbor(oRoute1->at(iContOrder), iContRoute1);




                }



            }

            if(hasImprove){

            }


        }


    }
}
