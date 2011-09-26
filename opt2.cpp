#include "opt2.h"
#include "depot.h"
#include "route.h"
#include <QDebug>

Opt2::Opt2(SOD *opSolution)
{
    this->opSolution = opSolution;
}

void Opt2::performeMove(){

    Depot *depotAux = this->opSolution->getDepot(0);
    int iFlagOp = 0;
    int iFlagInverserOrder4 = 0;
    float nNewCost = 0.0;
    float nCurrentCost = this->opSolution->getCostSolution();
    int iBestOrder1 = 0, iBestOrder2 = 0, iBestOrder3 = 0, iBestOrder4 = 0;
    int iOrder1 = 0, iOrder2 = 0, iOrder3 = 0, iOrder4 = 0;
    QList<Route*> listRoutes = depotAux->getRoutes();


    for(int iContRoutes = 0; iContRoutes < listRoutes.size(); iContRoutes++){

        Route *r1 = listRoutes.at(iContRoutes);
        QList<int> *r1Rota =  r1->getRoute();
        bool hasImprove = true;

        while(hasImprove){

            hasImprove = false;
            iFlagInverserOrder4 = -1;
            for(int i = -1; i < r1Rota->size() - 1; i++){
                iOrder1 = i;
                iOrder2 = i + 1;

                for(int j = 1; j < r1Rota->size()-1; j++){
                    iOrder3 = j;
                    iOrder4 = (j + 1 >= r1Rota->size()) ? -1 : j + 1;
                    //iOrder4 = j + 1;

                    if(iOrder1 == -1 && iOrder4 == -1){
                        break;
                    }

                    //Sequential
                    r1Rota->swap(iOrder2, iOrder3);
                    nNewCost = this->opSolution->getCostSolution();
                    if(nNewCost < nCurrentCost){
                        //This flag mark sequential move.
                        iFlagOp = 1;
                        nCurrentCost = nNewCost;
                        iBestOrder1 = iOrder1;
                        iBestOrder2 = iOrder2;
                        iBestOrder3 = iOrder3;
                        iBestOrder4 = iOrder4;
                        hasImprove = true;
                    }

                    r1Rota->swap(iOrder3, iOrder2);

                    if(iOrder4 == -1){
                        //When iOrder == -1, note that occurs a situation where is not possible perform a normal swap because the index is -1.
                        //When it occur, its realized a different swap than simule the inverser exchange normaly.
                        //swap (1,3) then swap(1,2) - Its used a iFlagInverserOrder4 to flag when occurs.

                        r1Rota->swap(iOrder1, iOrder3);
                        r1Rota->swap(iOrder1, iOrder2);
                        nNewCost = this->opSolution->getCostSolution();
                        qDebug() << nNewCost << " < " << nCurrentCost;
                        if(nNewCost < nCurrentCost){
                            //This flag mark inverse move.
                            iFlagOp = 2;
                            iFlagInverserOrder4 = 1;
                            nCurrentCost = nNewCost;
                            iBestOrder1 = iOrder1;
                            iBestOrder2 = iOrder2;
                            iBestOrder3 = iOrder3;
                            iBestOrder4 = iOrder4;
                            hasImprove = true;
                        }
                        r1Rota->swap(iOrder2, iOrder1);
                        r1Rota->swap(iOrder3, iOrder1);

                    }else{

                        //Normal Inverser
                        r1Rota->swap(iOrder2, iOrder4);
                        r1Rota->swap(iOrder3, iOrder4);
                        nNewCost = this->opSolution->getCostSolution();
                        if(nNewCost < nCurrentCost){
                            //This flag mark inverse move.
                            iFlagOp = 2;
                            nCurrentCost = nNewCost;
                            iBestOrder1 = iOrder1;
                            iBestOrder2 = iOrder2;
                            iBestOrder3 = iOrder3;
                            iBestOrder4 = iOrder4;
                            hasImprove = true;
                        }
                        r1Rota->swap(iOrder4, iOrder3);
                        r1Rota->swap(iOrder4, iOrder2);

                    }

                }
            }

            if(hasImprove){
                if(iFlagOp == 1){
                    r1Rota->swap(iBestOrder2, iBestOrder3);
                }else if(iFlagOp == 2){

                    if(iFlagInverserOrder4 == 1){
                        r1Rota->swap(iBestOrder1, iBestOrder3);
                        r1Rota->swap(iBestOrder1, iBestOrder2);
                    }else{
                        r1Rota->swap(iBestOrder2, iBestOrder4);
                        r1Rota->swap(iBestOrder3, iBestOrder4);
                    }
                }
            }

            iFlagOp = -1;

        }
    }
}








