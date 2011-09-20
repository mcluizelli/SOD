#include "sbas.h"
#include "opt2.h"
#include <QDebug>
#include <qmath.h>

bool compareDecreseSaving(SavingAS saving1, SavingAS saving2){
    if(saving1.nValue > saving2.nValue) return true;
    else return false;
}

SbAS::SbAS(SOD oSodInitialSolution, PheromoneInf *oPheromoneInf, int iNumTotalAnts , float nAlpha, float nBeta, float nRo, int iNumSolutionElitist, int iNumTotalAntsSubProblem)
{

    //Tuning parameter based on reimman paper.

    this->iKNeighbor              = oSodInitialSolution.getNumOrders() / 4;
    this->nAlpha                  = nAlpha;
    this->nBeta                   = nBeta;
    this->nRo                     = nRo;
    this->iNumTotalAnts           = iNumTotalAnts;
    this->iNumTotalAntsSubProblem = iNumTotalAntsSubProblem;
    this->iNumSulutionElitist     = iNumSolutionElitist;
    this->oSodInitialSolution     = oSodInitialSolution;
    this->oPheromoneConcentration = oPheromoneInf;
    this->nCostBestSolution       = (float)RAND_MAX;
    qsrand(time(NULL));

/*
    this->nAlpha = 5;
    this->nBeta  = 5;
    this->nRo    = 0.95;
    this->iNumTotalAnts = oSodInitialSolution.getNumOrders();
    this->iNumSulutionElitist = 3;

  */
}

void SbAS::saveSolution(SOD *oSolution){

    float nCostSolution = oSolution->getCostSolution();
    float nCostAux= 0.0;
    int   iIndex = 0;

    if(this->oSodElitistListSolution.size() < this->iNumSulutionElitist){
        for(int iCont = 0; iCont < this->oSodElitistListSolution.size(); iCont++){
            nCostAux = this->oSodElitistListSolution.at(iCont)->getCostSolution();
            iIndex = iCont;
            if(nCostSolution <= nCostAux){
                break;
            }
        }
        this->oSodElitistListSolution.insert(iIndex, oSolution);
    }else{
        for(int iCont = 0; iCont < this->oSodElitistListSolution.size(); iCont++){
            nCostAux = this->oSodElitistListSolution.at(iCont)->getCostSolution();
            iIndex = iCont;
            if(nCostSolution <= nCostAux){
                break;
            }
        }
        this->oSodElitistListSolution.insert(iIndex, oSolution);
        if(this->oSodElitistListSolution.size() == this->iNumSulutionElitist){
            this->oSodElitistListSolution.removeLast();
        }
    }
}

SOD* SbAS::run(){

    float nCostCurrentSolution = 0.0;

    for(int iCont = 0; iCont < this->iNumTotalAnts; iCont++){

        qDebug() << iCont;
        for(int iAnts = 0; iAnts < this->iNumTotalAnts; iAnts++){
            SOD *oSolution = stepConstrutive();
            stepLocalSerch(oSolution);
            //Lista de solucoes elite.
            saveSolution(oSolution);
        }
        nCostCurrentSolution = this->oSodElitistListSolution.at(0)->getCostSolution();

        if( nCostCurrentSolution < nCostBestSolution){
            nCostBestSolution = nCostCurrentSolution;
            this->oSODBestSolution = this->oSodElitistListSolution.at(0);
        }

        //Utiliza-se a lista de solucoes de elite para realizar a atualizacao de pheromone.
        stepUpdatePheromone();

    }

    return this->oSODBestSolution;
}

SOD* SbAS::runForSub(){
    //This runForSub is diferent to run() because dont run the step UpdatePheromone. Its used when is call to subproblem of D-Ants algorithm.
    for(int iAnts = 0; iAnts < this->iNumTotalAntsSubProblem; iAnts++){
        SOD *oSolution = stepConstrutive();
        stepLocalSerch(oSolution);
        //Lista de solucoes elite.
        saveSolution(oSolution);
    }
    this->oSODBestSolution = this->oSodElitistListSolution.at(0);

    return this->oSODBestSolution;
}

void SbAS::stepUpdatePheromone(){
    int iNumOrders = this->oSodInitialSolution.getNumOrders();
    float nValueAux = 0.0;
    float nValueNew = 0.0;
    float nCostBestSolution = this->oSodElitistListSolution.at(0)->getCostSolution();

    //All arcs are avaporated.
    for(int iCont1 = 0; iCont1 < iNumOrders; iCont1++){
        for(int iCont2 = 0; iCont2 < iNumOrders; iCont2++){
            nValueAux = this->oPheromoneConcentration->getPheromoneArc(iCont1, iCont2);
            //Evaporation Rate.
            this->oPheromoneConcentration->setPheromoneArc(iCont1, iCont2, nValueAux * this->nRo);
        }
    }

    //Update best solution
    for(int iContDepot = 0; iContDepot < this->oSODBestSolution->getNumberDepot(); iContDepot++){

        Depot *oDepotAux = this->oSODBestSolution->getDepot(iContDepot);
        QList<Route*> oRoutesAux = oDepotAux->getRoutes();

        for(int iContRoute = 1; iContRoute < oRoutesAux.size(); iContRoute++){

            QList<int> *oOrders = oRoutesAux.at(iContRoute)->getRoute();
            nValueAux = this->oPheromoneConcentration->getPheromoneArc(oDepotAux->getIndexOfOrder(), oOrders->at(0));
            nValueNew = nValueAux + this->iNumSulutionElitist * (float(1)/nCostBestSolution);
            this->oPheromoneConcentration->setPheromoneArc(oDepotAux->getIndexOfOrder(), oOrders->at(0), nValueNew);
            for(int iContOrders; iContOrders < oOrders->size() - 1; iContOrders++){

                nValueAux = this->oPheromoneConcentration->getPheromoneArc(oOrders->at(iContOrders), oOrders->at(iContOrders + 1));
                nValueNew = nValueAux + this->iNumSulutionElitist * (float(1)/nCostBestSolution);
                this->oPheromoneConcentration->setPheromoneArc(oOrders->at(iContOrders), oOrders->at(iContOrders + 1), nValueNew);

            }

            nValueAux = this->oPheromoneConcentration->getPheromoneArc(oOrders->at(oOrders->size()-1), oDepotAux->getIndexOfOrder());
            nValueNew = nValueAux + this->iNumSulutionElitist * (float(1)/nCostBestSolution);
            this->oPheromoneConcentration->setPheromoneArc(oOrders->at(oOrders->size()-1), oDepotAux->getIndexOfOrder(), nValueNew);


        }

    }

    //Update of rth best solution.
    for(int iContSolution = 1; iContSolution < iNumSulutionElitist; iContSolution++){

        SOD *oSolutionAux = this->oSodElitistListSolution.at(iContSolution);
        float nCostSolution = oSolutionAux->getCostSolution();
        for(int iContDepot = 0; iContDepot < oSolutionAux->getNumberDepot(); iContDepot++){

            Depot *oDepotAux = oSolutionAux->getDepot(iContDepot);
            QList<Route*> oRoutesAux = oDepotAux->getRoutes();

            for(int iContRoute = 1; iContRoute < oRoutesAux.size(); iContRoute++){

                QList<int> *oOrders = oRoutesAux.at(iContRoute)->getRoute();
                nValueAux = this->oPheromoneConcentration->getPheromoneArc(oDepotAux->getIndexOfOrder(), oOrders->at(0));
                nValueNew = nValueAux + (iNumSulutionElitist-iContSolution)/nCostSolution;
                this->oPheromoneConcentration->setPheromoneArc(oDepotAux->getIndexOfOrder(), oOrders->at(0), nValueNew);
                for(int iContOrders; iContOrders < oOrders->size() - 1; iContOrders++){

                    nValueAux = this->oPheromoneConcentration->getPheromoneArc(oOrders->at(iContOrders), oOrders->at(iContOrders + 1));
                    nValueNew = nValueAux + (iNumSulutionElitist-iContSolution)/nCostSolution;
                    this->oPheromoneConcentration->setPheromoneArc(oOrders->at(iContOrders), oOrders->at(iContOrders + 1), nValueNew);

                }

                nValueAux = this->oPheromoneConcentration->getPheromoneArc(oOrders->at(oOrders->size()-1), oDepotAux->getIndexOfOrder());
                nValueNew = nValueAux + (iNumSulutionElitist-iContSolution)/nCostSolution;
                this->oPheromoneConcentration->setPheromoneArc(oOrders->at(oOrders->size()-1), oDepotAux->getIndexOfOrder(), nValueNew);


            }

        }

    }

}

void SbAS::stepLocalSerch(SOD *oSolution){
    Opt2 opt(oSolution);
    opt.performeMove();
}

int SbAS::returnAtractivenessList(QList<SavingAS> *oListSaving, Depot *oDepotAux){

    //In this list, its append the n feasible savings.
    float nCapacity = oDepotAux->getCapacity();
    float nSumAtractivenessNeighbor = 0;
    float nSumAtractivenessNeighborNormalized = 0;
    int iContNeighbor = 0;
    QList<AtractivenessItem> oAtractivenessList;
    QList<float> oAtractivenessValue;


    for(int iContSaving = 0; iContSaving < oListSaving->size(); iContSaving++){

        SavingAS oSavingAux = oListSaving->at(iContSaving);
        //Remover estas rotass e incluir uma com as duas.

        int iIndexRoute1 = oDepotAux->getRouteByOrder(oSavingAux.iOrder1);
        int iIndexRoute2 = oDepotAux->getRouteByOrder(oSavingAux.iOrder2);

        Route *r1 = oDepotAux->getRoute(iIndexRoute1);
        Route *r2 = oDepotAux->getRoute(iIndexRoute2);

        int nDemandRoute1 = this->oSodInitialSolution.getDemandRoute(r1);
        int nDemandRoute2 = this->oSodInitialSolution.getDemandRoute(r2);

        //Garante que as ordens sejam nos extremos das rotas e as restrições da modelagem do vrpc.
        if( (nDemandRoute1 + nDemandRoute2 <= nCapacity) && (iIndexRoute1 != iIndexRoute2) && (r1->getRoute()->at(0) == oSavingAux.iOrder1 || r1->getRoute()->at(r1->getRoute()->size() - 1) == oSavingAux.iOrder1)
             &&
             (r2->getRoute()->at(0) == oSavingAux.iOrder2 || r2->getRoute()->at(r2->getRoute()->size() - 1) == oSavingAux.iOrder2) ){
            AtractivenessItem oItem;
            oItem.iItem = oSavingAux;
            oItem.iIndex = iContSaving;

            oAtractivenessList.append(oItem);
            iContNeighbor++;

        }else{
            oListSaving->removeAt(iContSaving);
            iContSaving--;
        }
        if(iContNeighbor == iKNeighbor) break;
    }

    for(int iCont = 0; iCont < oAtractivenessList.size(); iCont++){
        nSumAtractivenessNeighbor += oAtractivenessList.at(iCont).iItem.nValue;
    }

    float nAtractivenessAux = 0.0;
    for(int iCont = 0; iCont < oAtractivenessList.size(); iCont++){
        nAtractivenessAux = oAtractivenessList.at(iCont).iItem.nValue/nSumAtractivenessNeighbor;
        oAtractivenessValue.append(nAtractivenessAux);
        nSumAtractivenessNeighborNormalized += nAtractivenessAux;
    }

    //Generate a pseudo random number.
    float nRand = (float)qrand() / (float)RAND_MAX;
    float nSum = 0.0;
    int iIndex = 0;
    for(int iCont = 0; iCont < oAtractivenessValue.size(); iCont++){

        nSum += oAtractivenessValue.at(iCont);
        if(nRand < nSum){
            iIndex = oAtractivenessList.at(iCont).iIndex;
            break;
        }

    }

    return iIndex;
}

SOD* SbAS::stepConstrutive(){

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
        //qDebug() << "Building solution";

        //Arrumar este for... condicao de parada. ARRUMAR
        for(int iContSaving = 0; iContSaving < oListSaving.size(); iContSaving++){

            int in = returnAtractivenessList(&oListSaving, oDepotAux);
            if(oListSaving.size() == 0) break;
            //SavingAS oSavingAux = oListSaving.at(iContSaving);
            SavingAS oSavingAux = oListSaving.at(in);

            //Remover estas rotas e incluir uma com as duas.
            //qDebug() << "Trying " << oSavingAux.iOrder1 << "," << oSavingAux.iOrder2;
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
                //qDebug() << "infeasible saving";
            }

            oListSaving.removeAt(in);
            iContSaving--;



        }
    }
    return oCopySolution;
}

QList<SavingAS> SbAS::buildSavingList(Depot *oDepot){

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
                oItemSaving.nValue = qPow(nSavingAux, this->nBeta) * qPow(this->oPheromoneConcentration->getPheromoneArc(iIndexOrder1,iIndexOrder2), this->nAlpha);
                oSavingList.append(oItemSaving);

            }
        }
    }

    qSort(oSavingList.begin(), oSavingList.end(), compareDecreseSaving);

    return oSavingList;

}
