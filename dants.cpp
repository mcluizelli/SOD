#include "dants.h"
#include "sbas.h"
#include "sod.h"
#include "sweepalgorithm.h"
#include "qmath.h"
#include "gui/showroutes.h"
#include <QDebug>

bool comparePolarDAnts(OrderPolar order1, OrderPolar order2){

    if( order1.nAngule < order2.nAngule ) return true;
    else return false;

}

DAnts::DAnts(SOD oSodInitialSolution, int iNumMaxIterationDAnts)
{
    this->iNs = 3;
    this->nCostBestSolution = (float)9999999999;
    this->oSodInitialSolution = oSodInitialSolution;
    this->iNumMaxIterationDAnts = iNumMaxIterationDAnts;
    this->oPheromoneInf = new PheromoneInf(&oSodInitialSolution);
}

SOD* DAnts::run(){

    float nCostAux = 0.0;

    for(int iCont1 = 0; iCont1 < iNumMaxIterationDAnts; iCont1++){

        //First: Apply SbAS to solve instance;
        SbAS *oSbAS = new SbAS(this->oSodInitialSolution, oPheromoneInf, this->oSodInitialSolution.getNumOrders(), 5, 5, 0.95, 3);
        //Return the best solution found so far.solution quality
        SOD *oCurrentSolution = oSbAS->run();

        //Here start the application of D-Ants.
        for(int iContDepot = 0; iContDepot < oCurrentSolution->getNumberDepot(); iContDepot++){
            //Calcule the center of gravity to each route.
            QList<RouteCenterGravity> oCentersGravity = calculeCenterGravityToRoutes(oCurrentSolution->getDepot(iContDepot));
            //Follow the D-Ants algorithm.
            QList<SOD*> oListSubProblems = sweepAlgorithmModified(oCentersGravity, oCurrentSolution, iContDepot);
            QList<SOD*> oListNewSolution = applySbASCluster(oListSubProblems);
            compareResults(iContDepot, oCurrentSolution, oListNewSolution);

        }

        //Falta atualizar a pheromone.

        nCostAux = oCurrentSolution->getCostSolution();

        if(nCostAux < nCostBestSolution){
            nCostBestSolution = nCostAux;
            oBestSolution = oCurrentSolution;
        }

        this->oRouteIndexByCluster.clear();

    }

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


            //qDebug() << "add route a cluster";
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

            //qDebug() << "salva sub";
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

    for(int i = 0; i < oListSubProblems.size(); i++){\
        //Call SbAS for solve the subproblems.
        SbAS *sb = new SbAS(*oListSubProblems.at(i), this->oPheromoneInf, 100, 5, 5, 0.95, 3) ;
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

    for(int iContSubProblem = 0; iContSubProblem < oListSubProblems.size(); iContSubProblem++){
        nNewCostSubProblem = 0.0, nCurrentCost = 0.0;
        SOD* oSubProblem   = oListSubProblems.at(iContSubProblem);
        nNewCostSubProblem = oSubProblem->getCostSolution();
        for(int i = 0; i < this->oRouteIndexByCluster.at(iContSubProblem).oListIndexRoutes.size(); i++){
            nCurrentCost += oSolution->getCostRouteByDepot(iDepot, this->oRouteIndexByCluster.at(iContSubProblem).oListIndexRoutes.at(i) );
        }

        nNewCostSubProblem = oSubProblem->getCostSolution();

        //qDebug() << nCurrentCost << " - " << nNewCostSubProblem;

        if(nCurrentCost > nNewCostSubProblem){
            //Rota nova é melhor, entao removo a anterior e insiro a nova.
            nCostFinal += nNewCostSubProblem;
            //remove
            /*

            for(int i = 0; i < this->oRouteIndexByCluster.at(iContSubProblem).oListIndexRoutes.size(); i++){
                oDepotAux->removeRoute(this->oRouteIndexByCluster.at(iContSubProblem).oListIndexRoutes.at(i));
            }
            //add
            Route *oNewRoute = Route();
            oNewRoute->addOrder();
            oDepotAux->addRoute();
            */
        }else{
            nCostFinal += nCurrentCost;
        }

    }

    qDebug() << "Novo: " << nCostFinal << " Anterior: " << oSolution->getCostSolution();
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



