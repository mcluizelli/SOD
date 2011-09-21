#include "pheromoneinf.h"
#include "sod.h"
#include <QDebug>

PheromoneInf::PheromoneInf(SOD *oSODSolution)
{
    this->iNumOrders = oSODSolution->getNumOrders() + oSODSolution->getNumberDepot();
    this->npPheromoneConcentration = new float*[iNumOrders];
    for(int iCont = 0; iCont < iNumOrders; iCont++)
        this->npPheromoneConcentration[iCont] = new float[iNumOrders];

    //In the begin, all arcs have value equal to 1 for pheromone concentration.
    for(int iCont1 = 0; iCont1 < iNumOrders; iCont1++){
        for(int iCont2 = 0; iCont2 < iNumOrders; iCont2++){
            this->npPheromoneConcentration[iCont1][iCont2] = 1;
        }
    }
}

float PheromoneInf::getPheromoneArc(int iIArc, int iJArc){
    return this->npPheromoneConcentration[iIArc][iJArc];
}

void  PheromoneInf::setPheromoneArc(int iIArc, int iJArc, float nValue){
    this->npPheromoneConcentration[iIArc][iJArc] = nValue;
}

void PheromoneInf::showInf(){
    for(int iCont1 = 0; iCont1 < iNumOrders; iCont1++){
        for(int iCont2 = 0; iCont2 < iNumOrders; iCont2++){
            qDebug() << iCont1 << "," << iCont2 << " - " << this->npPheromoneConcentration[iCont1][iCont2];
        }
    }
}
