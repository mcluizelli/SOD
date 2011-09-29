#include "dants.h"
#include "sbas.h"
#include "sod.h"
#include "sweepalgorithm.h"
#include "qmath.h"
#include "gui/showroutes.h"
#include <QDebug>
#include <QTime>

bool comparePolarDAnts(OrderPolar order1, OrderPolar order2){

    if( order1.nAngule < order2.nAngule ) return true;
    else return false;

}

DAnts::DAnts(SOD oSodInitialSolution, int iNumMaxMilSecRun)
{
    this->iNs = 3;
    this->nCostBestSolution = (float)RAND_MAX;
    this->oSodInitialSolution = oSodInitialSolution;
    this->iNumMaxMilSecRun = iNumMaxMilSecRun;
    this->oPheromoneInf = new PheromoneInf(&oSodInitialSolution);
}

SOD* DAnts::run(){

    float nCostAux = 0.0;
    QTime oTempo;
    oTempo.start();

    //while(oTempo.elapsed() < this->iNumMaxMilSecRun){
    for(int i = 0; i < 20; i++){

        //First: Apply SbAS to solve instance;
        SbAS *oSbAS = new SbAS(this->oSodInitialSolution, oPheromoneInf, this->oSodInitialSolution.getNumOrders()
                               ,5, 5, 0.95, 5, 75);
        //Return the best solution found so far.solution quality
        SOD *oCurrentSolution = oSbAS->run();
        //oBestSolution = oCurrentSolution;
        //qDebug() << oBestSolution->getCostSolution();
        //Here start the application of D-Ants.
        for(int iContDepot = 0; iContDepot < oCurrentSolution->getNumberDepot(); iContDepot++){
            //Calcule the center of gravity to each route.
            QList<RouteCenterGravity> oCentersGravity = calculeCenterGravityToRoutes(oCurrentSolution->getDepot(iContDepot));
            //Follow the D-Ants algorithm.
            QList<SOD*> oListSubProblems = sweepAlgorithmModified(oCentersGravity, oCurrentSolution, iContDepot);
            //A cada cluster aplica-se o SbAS
            QList<SOD*> oListNewSolution = applySbASCluster(oListSubProblems);
            compareResults(iContDepot, oCurrentSolution, oListNewSolution);

        }

        //Falta atualizar a pheromone.

        nCostAux = oCurrentSolution->getCostSolution();

        if(nCostAux < nCostBestSolution){
            nCostBestSolution = nCostAux;
            oBestSolution = oCurrentSolution;
            updatePheromone(oBestSolution);
        }
        qDebug() << nCostBestSolution;
        this->oRouteIndexByCluster.clear();

    }
    qDebug() << "Best Cost: " << nCostBestSolution;
    return oBestSolution;

}

QList<SOD*> DAnts::sweepAlgorithmModified(QList<RouteCenterGravity> oCentersGravity, SOD *oCurrentSolution, int iContDepot){

    //1- Define n-s,t - number tour to cluster. ns = number of cluster.
    //Clustering.
    //We choose a starting node randomly (center of gravity).
    /*Passos - Seleciona um ponto aleatorio.
             - Seleciona proximo o primeiro centro de gravidade apos o ponto.(coord. polar).
             - Os nos deste serao add.
             - Sucessivamente escolher proximo centro. Add pontos aos agrupamentos.
    */

    int iNst          = qCeil((float)oCentersGravity.size()/(float)this->iNs);
    int iNodeSeed     = rand() % oCurrentSolution->getNumOrders();
    Depot *opDepotAux = this->oSodInitialSolution.getDepot(iContDepot);

    //Return a copy of orders alocated.
    QList<int> optOrderAux = opDepotAux->getOrdersAlocated();

    //Create a list that conten index and angule of orders.
    QList<OrderPolar> opListOrderPolar;

    int iCoordXDepot = opDepotAux->getCoordX();
    int iCoordYDepot = opDepotAux->getCoordY();
    int iDeltaX      = 0;
    int iDeltaY      = 0;

    //Calcula-se as coordenadas polares dos centros de gravidade.
    for( int iCont = 0; iCont < oCentersGravity.size(); iCont++){

        Order oCenter = oCentersGravity.at(iCont).oCenterGravity;
        OrderPolar op;
        op.iIndex = iCont;

        iDeltaX = oCenter.getCoordX() - iCoordXDepot;
        iDeltaY = oCenter.getCoordY() - iCoordYDepot;

        op.nAngule = (qAtan2((double)(iDeltaY),(double)(iDeltaX)));

        opListOrderPolar.append(op);

    }

    qSort(opListOrderPolar.begin(), opListOrderPolar.end(), comparePolarDAnts);

    //Calcula-se a coordena polar do nó seed escolhido aleatoriamente.
    iDeltaX = this->oSodInitialSolution.getOrder(iNodeSeed)->getCoordX() - iCoordXDepot;
    iDeltaY = this->oSodInitialSolution.getOrder(iNodeSeed)->getCoordY() - iCoordYDepot;
    float nPolarNodeSeed = (qAtan2((double)(iDeltaY),(double)(iDeltaX)));
    int iIndexFirstCluster = 0;

    for(int iCont = 0; iCont < opListOrderPolar.size(); iCont++){
        if(nPolarNodeSeed < opListOrderPolar.at(iCont).nAngule){
            iIndexFirstCluster = iCont;
            break;
        }
    }

    //A partir daqui criar os Ns cluster de solucoes para serem resolvidos pelo SbAS.
    int iCont = iIndexFirstCluster;
    int iContNst = 0;

    //Crio uma lista de sub-problemas. (Cluster).
    QList<SOD*> oListSubProblems;

    QList<int> *oCluster = new QList<int>;
    RouteIndexByCluster oRouteIndexCluster;

    for(int i = 0; i < oCentersGravity.size() + 1;){

        if(iContNst < iNst && i < oCentersGravity.size()){

            for(int iOrder = 0; iOrder < oCentersGravity.at(iCont).oRoute->getRoute()->size(); iOrder++){
                oCluster->append(oCentersGravity.at(iCont).oRoute->getRoute()->at(iOrder));
            }
            //Incrementa Nst (Numero de rotas por cluster).
            oRouteIndexCluster.oListIndexRoutes.append(iCont);
            iContNst++;
            i++;
            iCont++;
            iCont = iCont % oCentersGravity.size();

        }else{

            SOD *oSubProblemN = this->oSodInitialSolution.copy(oCluster, opDepotAux);

            this->oRouteIndexByCluster.append(oRouteIndexCluster);
            oRouteIndexCluster.oListIndexRoutes.clear();

            oCluster->clear();
            //Zera contagem de rotas para cada cluster.
            iContNst = 0;
            oListSubProblems.append(oSubProblemN);
            if(i == oCentersGravity.size()) break;

        }

    }

    return oListSubProblems;


}

QList<SOD*> DAnts::applySbASCluster(QList<SOD*> oListSubProblems){

    //Apply the SbAS algorithm each subproblem.
    QList<SOD*> oListNewSolution;

    for(int i = 0; i < oListSubProblems.size(); i++){
        //Call SbAS for solve the subproblems.
        SbAS *sb = new SbAS(*oListSubProblems.at(i), this->oPheromoneInf
                            ,oListSubProblems.at(i)->getDepot(0)->getNumOrdersAlocated() , 5, 5, 0.95, 3, 50);
        oListNewSolution.append(sb->runForSub());
    }

    //After the solution for subproblem has been constructed, we compare the new results with the previous.
    //If the new results are better than the previous results, then its modified on incumbent solution.
    return oListNewSolution;
}

void DAnts::compareResults(int iDepot, SOD *oSolution, QList<SOD*> oListSubProblems){

    float nNewCostSubProblem = 0.0, nCurrentCost = 0.0;
    float nCostFinal = 0.0;
    Depot *oDepotAux = oSolution->getDepot(iDepot);
    QList<int> oRouteToRemove;
    QList<SOD*> oRoutesToAdd;

    for(int iContSubProblem = 0; iContSubProblem < oListSubProblems.size(); iContSubProblem++){

        nNewCostSubProblem = 0.0, nCurrentCost = 0.0;
        SOD* oSubProblem   = oListSubProblems.at(iContSubProblem);
        nNewCostSubProblem = oSubProblem->getCostSolution();

        for(int i = 0; i < this->oRouteIndexByCluster.at(iContSubProblem).oListIndexRoutes.size(); i++){
            nCurrentCost += oSolution->getCostRouteByDepot(iDepot, this->oRouteIndexByCluster.at(iContSubProblem).oListIndexRoutes.at(i) );
        }


        if(nCurrentCost > nNewCostSubProblem){
            //Rota nova é melhor, entao removo a anterior e insiro a nova.
            nCostFinal += nNewCostSubProblem;
            oRouteToRemove.append(this->oRouteIndexByCluster.at(iContSubProblem).oListIndexRoutes);
            oRoutesToAdd.append(oSubProblem);
        }else{
            nCostFinal += nCurrentCost;
        }

    }

    //Do all modifications

    //Remove all mark routes on the solution.
    qSort(oRouteToRemove);
    for(int i = oRouteToRemove.size() - 1; i >= 0; i--){
        oDepotAux->removeRoute(oRouteToRemove.at(i));
    }

    //Add new Routes.
    for(int iContRoutesAdd = 0; iContRoutesAdd < oRoutesToAdd.size(); iContRoutesAdd++){

        Depot *oDeptoNewRoute =  oRoutesToAdd.at(iContRoutesAdd)->getDepot(0);
        QList<Route*> oRoutes = oDeptoNewRoute->getRoutes();
        for(int iContRoute = 0; iContRoute < oRoutes.size() ; iContRoute++){

            QList<int> *oOrders = oRoutes.at(iContRoute)->getRoute();
            Route *newRoute = new Route;
            for(int iContOrder = 0; iContOrder < oOrders->size(); iContOrder++){
                newRoute->addOrder(oOrders->at(iContOrder));
            }
            oDepotAux->addRoute(newRoute);
        }
    }


}

void DAnts::updatePheromone(SOD *oSODBestSolution){

    int iNumElitist = 6;
    float nValueAux = 0.0, nValueNew = 0.0;

    for(int iContDepot = 0; iContDepot < oSODBestSolution->getNumberDepot(); iContDepot++){

        Depot *oDepotAux = oSODBestSolution->getDepot(iContDepot);
        QList<Route*> oRoutesAux = oDepotAux->getRoutes();

        for(int iContRoute = 1; iContRoute < oRoutesAux.size(); iContRoute++){

            QList<int> *oOrders = oRoutesAux.at(iContRoute)->getRoute();
            nValueAux = this->oPheromoneInf->getPheromoneArc(oDepotAux->getIndexOfOrder(), oOrders->at(0));
            nValueNew = nValueAux + iNumElitist * (float(1)/nCostBestSolution);
            this->oPheromoneInf->setPheromoneArc(oDepotAux->getIndexOfOrder(), oOrders->at(0), nValueNew);
            for(int iContOrders; iContOrders < oOrders->size() - 1; iContOrders++){

                nValueAux = this->oPheromoneInf->getPheromoneArc(oOrders->at(iContOrders), oOrders->at(iContOrders + 1));
                nValueNew = nValueAux + iNumElitist * (float(1)/nCostBestSolution);
                this->oPheromoneInf->setPheromoneArc(oOrders->at(iContOrders), oOrders->at(iContOrders + 1), nValueNew);

            }

            nValueAux = this->oPheromoneInf->getPheromoneArc(oOrders->at(oOrders->size()-1), oDepotAux->getIndexOfOrder());
            nValueNew = nValueAux + iNumElitist * (float(1)/nCostBestSolution);
            this->oPheromoneInf->setPheromoneArc(oOrders->at(oOrders->size()-1), oDepotAux->getIndexOfOrder(), nValueNew);


        }

    }
}

QList<RouteCenterGravity> DAnts::calculeCenterGravityToRoutes(Depot *oDepot){

    QList<RouteCenterGravity> oListRouteCenterGravity;
    QList<Route*> oListRoute = oDepot->getRoutes();

    for(int iContRoutes = 0; iContRoutes < oListRoute.size(); iContRoutes++){
        RouteCenterGravity     oCenter;
        Order oCenterGravity   = this->oSodInitialSolution.calculeCenterGravity(oListRoute.at(iContRoutes));
        oCenter.oCenterGravity = oCenterGravity;
        oCenter.oRoute         = oListRoute.at(iContRoutes);
        oListRouteCenterGravity.append(oCenter);
    }

    return oListRouteCenterGravity;

}



