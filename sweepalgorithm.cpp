#include "sweepalgorithm.h"
#include "sod.h"
#include "depot.h"
#include "opt2.h"
#include "order.h"
#include <QDebug>
#include <qmath.h>


bool comparePolar(OrderPolar order1, OrderPolar order2){

    if( order1.nAngule < order2.nAngule ) return true;
    else return false;

}

SweepAlgorithm::SweepAlgorithm(SOD oSolution)
{
    this->oSodInitialSolution = oSolution;
}

void SweepAlgorithm::run(){
    stepConstrutive();
    stepLocalSearch();
}

void SweepAlgorithm::stepLocalSearch(){
    qDebug() << "Call 2-opt";
    Opt2 opt(&this->oSodInitialSolution);
    opt.performeMove();
}

void SweepAlgorithm::stepConstrutive(){

    int iDeltaX      = 0;
    int iDeltaY      = 0;
    int iCoordXDepot = 0;
    int iCoordYDepot = 0;

    //For each depot in SOD, its applied this construtive step.
    for(int iCont = 0; iCont < this->oSodInitialSolution.getNumberDepot(); iCont++){

        //get the depot.
        Depot *opDepotAux = this->oSodInitialSolution.getDepot(iCont);
        //Return a copy of orders alocated.
        QList<int> optOrderAux = opDepotAux->getOrdersAlocated();
        //Create a list that conten index and angule of orders.
        QList<OrderPolar> opListOrderPolar;

        iCoordXDepot = opDepotAux->getCoordX();
        iCoordYDepot = opDepotAux->getCoordY();

        for( int iCont2 = 0; iCont2 < optOrderAux.size(); iCont2++){

            //Como as ordens alocadas para cada deposito incluem o proprio deposito como uma ordem na lista, precisa-se exclui-la do processo de roteamento.
            int iOrder = optOrderAux.at(iCont2);

            OrderPolar op;
            op.iIndex = iOrder;

            iDeltaX = this->oSodInitialSolution.getOrder(iOrder)->getCoordX() - iCoordXDepot;
            iDeltaY = this->oSodInitialSolution.getOrder(iOrder)->getCoordY() - iCoordYDepot;

            op.nAngule = (qAtan2((double)(iDeltaY),(double)(iDeltaX)));

            opListOrderPolar.append(op);

        }

        //Sort orders.
        qSort(opListOrderPolar.begin(), opListOrderPolar.end(), comparePolar);

        //create routes
        int iCont2 = 0;

        while(iCont2 < opListOrderPolar.size()){

            Route *route = new Route;
            float nCapacityEquip = 0;

            while( iCont2 < opListOrderPolar.size() && nCapacityEquip + this->oSodInitialSolution.getOrder(opListOrderPolar.at(iCont2).iIndex)->getDemand()
                   <= opDepotAux->getCapacity()){

                int iOrder = opListOrderPolar.at(iCont2).iIndex;
                route->addOrder(iOrder);
                nCapacityEquip += this->oSodInitialSolution.getOrder(iOrder)->getDemand();
                iCont2++;

            }

            opDepotAux->addRoute(route);

        }

        opListOrderPolar.clear();

    }

}
