#include "savingsalgorithm.h"

bool compareDecreseSavingAlgorithm(SavingAS saving1, SavingAS saving2){
    if(saving1.nValue > saving2.nValue) return true;
    else return false;
}

SavingsAlgorithm::SavingsAlgorithm(SOD opSolution)
{
    this->oSodInitialSolution = opSolution;
}

QList<SavingAS> SavingsAlgorithm::buildSavingList(Depot *oDepot){

    QList<int> oOrderAlocatedDepot = oDepot->getOrdersAlocated();
    int   iNumOrders  = oOrderAlocatedDepot.size();
    int   iIndexDeptoInOrders = oDepot->getIndexOfOrder();
    int   iIndexOrder1 = 0, iIndexOrder2 = 0;
    float nSavingAux = 0.0;
    QList<SavingAS> oSavingList;

    for(int iCont1 = 0; iCont1 < iNumOrders; iCont1++){
        for(int iCont2 = iCont1; iCont2 < iNumOrders; iCont2++){

            if(iCont1 != iCont2){
                SavingAS oItemSaving;
                iIndexOrder1 = oOrderAlocatedDepot.at(iCont1);
                iIndexOrder2 = oOrderAlocatedDepot.at(iCont2);
                oItemSaving.iOrder1 = iIndexOrder1;
                oItemSaving.iOrder2 = iIndexOrder2;
                nSavingAux =  this->oSodInitialSolution.getDistance(iIndexDeptoInOrders, iIndexOrder1)
                              + this->oSodInitialSolution.getDistance(iIndexDeptoInOrders, iIndexOrder2)
                              - this->oSodInitialSolution.getDistance(iIndexOrder1, iIndexOrder2);
                oItemSaving.nValue = nSavingAux;
                oSavingList.append(oItemSaving);

            }
        }
    }

    qSort(oSavingList.begin(), oSavingList.end(), compareDecreseSavingAlgorithm);

    return oSavingList;

}

SOD SavingsAlgorithm::run(){

    SOD *oCopySolution = this->oSodInitialSolution.copy();

    //First: for each depot, aply the construtive step.
    int iNumDepot = oCopySolution->getNumberDepot();
    float nCapacity = 0.0;
    Depot *oDepotAux = NULL;

    for(int iContDepto = 0; iContDepto < iNumDepot; iContDepto++){

        oDepotAux = oCopySolution->getDepot(iContDepto);
        nCapacity = oDepotAux->getCapacity();
        QList<int> oOrdersAlocateted = oDepotAux->getOrdersAlocated();

        //Build trivial solution.
        for(int iCont = 0; iCont < oDepotAux->getNumOrdersAlocated(); iCont++){
            Route *r = new Route;
            r->addOrder(oOrdersAlocateted.at(iCont));
            oDepotAux->addRoute(r);
        }

        QList<SavingAS> oListSaving = buildSavingList(oDepotAux);
        //"Building solution";

        for(int iContSaving = 0; iContSaving < oListSaving.size(); iContSaving++){

            if(oListSaving.size() == 0) break;
            //SavingAS oSavingAux = oListSaving.at(iContSaving);
            SavingAS oSavingAux = oListSaving.at(iContSaving);

            //Remover estas rotas e incluir uma com as duas.
            // << "Trying " << oSavingAux.iOrder1 << "," << oSavingAux.iOrder2;
            int iIndexRoute1 = oDepotAux->getRouteByOrder(oSavingAux.iOrder1);
            int iIndexRoute2 = oDepotAux->getRouteByOrder(oSavingAux.iOrder2);

            Route *r1 = oDepotAux->getRoute(iIndexRoute1);
            Route *r2 = oDepotAux->getRoute(iIndexRoute2);
            Route *r3 = new Route;

            int nDemandRoute1 = oCopySolution->getDemandRoute(r1);
            int nDemandRoute2 = oCopySolution->getDemandRoute(r2);

            //Garante que as ordens sejam nos extremos das rotas.
            if( (nDemandRoute1 + nDemandRoute2 <= nCapacity) && (iIndexRoute1 != iIndexRoute2) && (r1->getRoute()->at(0) == oSavingAux.iOrder1 || r1->getRoute()->at(r1->getRoute()->size() - 1) == oSavingAux.iOrder1)
                 &&
                 (r2->getRoute()->at(0) == oSavingAux.iOrder2 || r2->getRoute()->at(r2->getRoute()->size() - 1) == oSavingAux.iOrder2) ){

                if(r1->getRoute()->at(0) == oSavingAux.iOrder1){
                    for(int iCont = r1->getRoute()->size() -1; iCont >= 0 ; iCont--){
                        r3->addOrder(r1->getRoute()->at(iCont));
                    }
                    if(r2->getRoute()->at(0) == oSavingAux.iOrder2){
                        for(int iCont = 0; iCont < r2->getRoute()->size(); iCont++){
                            r3->addOrder(r2->getRoute()->at(iCont));
                        }
                    }else{
                        for(int iCont = r2->getRoute()->size() - 1; iCont >= 0 ; iCont--){
                            r3->addOrder(r2->getRoute()->at(iCont));
                        }
                    }

                }else{
                    for(int iCont = 0; iCont < r1->getRoute()->size() ; iCont++){
                        r3->addOrder(r1->getRoute()->at(iCont));
                    }
                    if(r2->getRoute()->at(0) == oSavingAux.iOrder2){
                        for(int iCont = 0; iCont < r2->getRoute()->size(); iCont++){
                            r3->addOrder(r2->getRoute()->at(iCont));
                        }
                    }else{
                        for(int iCont = r2->getRoute()->size() - 1; iCont >= 0 ; iCont--){
                            r3->addOrder(r2->getRoute()->at(iCont));
                        }
                    }
                }

                oDepotAux->removeRoute(iIndexRoute1);
                oDepotAux->removeRoute(oDepotAux->getRouteByOrder(oSavingAux.iOrder2));

                oDepotAux->addRoute(r3);

            }else{
                //<< "infeasible saving";
            }

            oListSaving.removeAt(iContSaving);
            iContSaving--;


        }
    }
    return *oCopySolution;

}


