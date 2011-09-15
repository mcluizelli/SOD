#include "dants.h"
#include "sbas.h"
#include "sod.h"
#include "sweepalgorithm.h"
#include "qmath.h"
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

    for(int iCont1 = 0; iCont1 < /*iNumMaxIterationDAnts*/1; iCont1++){

        //Firs: Apply SbAS to instance;
        SbAS *oSbAS = new SbAS(this->oSodInitialSolution, oPheromoneInf);
        //Return the best solution found so far.
        SOD *oCurrentSolution = oSbAS->run();

        //Here start the application of D-Ants.
        for(int iContDepot = 0; iContDepot < oCurrentSolution->getNumberDepot(); iContDepot++){
            //Calcule the center of gravity to each route.
            QList<RouteCenterGravity> oCentersGravity = calculeCenterGravityToRoutes(oCurrentSolution->getDepot(iContDepot));
            //Follow the D-Ants algorithm.
            sweepAlgorithmModified(oCentersGravity, oCurrentSolution, iContDepot);
        }

        nCostAux = oCurrentSolution->getCostSolution();
        if(nCostAux < nCostBestSolution){
            nCostBestSolution = nCostAux;
            oBestSolution = oCurrentSolution;
        }

    }
    return oBestSolution;

}

void DAnts::sweepAlgorithmModified(QList<RouteCenterGravity> oCentersGravity, SOD *oCurrentSolution, int iContDepot){

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
    int iCont = iIndexFirstCluster + 1;
    int iContNst = 0;

    //Crio uma lista de sub-problemas. (Cluster).
    QList<SOD*> subProblems;

    QList<int> *oCluster = new QList<int>;
    for(int iOrder = 0; iOrder < oCentersGravity.at(iIndexFirstCluster).oRoute->getRoute()->size(); iOrder++){
        oCluster->append(oCentersGravity.at(iIndexFirstCluster).oRoute->getRoute()->at(iOrder));
    }

    while(iCont != iIndexFirstCluster){

        if(iContNst < iNst){

            for(int iOrder = 0; iOrder < oCentersGravity.at(iCont).oRoute->getRoute()->size(); iOrder++){
                oCluster->append(oCentersGravity.at(iCont).oRoute->getRoute()->at(iOrder));
            }

        }else{
            //Zera contagem de rotas para cada cluster.
            iContNst = 0;

        }

        iCont++;
        iCont = iCont % oCentersGravity.size();

    }

    qDebug() << "teste";




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



