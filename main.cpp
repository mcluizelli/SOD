#include <QtGui/QApplication>
#include <QDebug>
#include <QTime>
#include "inputcanonicalvrp.h"
#include "sod.h"
#include "sweepalgorithm.h"
#include "sbas.h"
#include "dants.h"
#include "gui/mainwindow.h"
#include "gui/showroutes.h"
#include "pheromoneinf.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //count the time.
    QTime oTempo;
    oTempo.start();

    //Read instance file.
    InputCanonicalVRP input;
    SOD vrp = input.read("vrpnc1.txt");

    //Alocated orders to depots.
    vrp.alocateOrderToDepots();

    //Apply SbAS algorithm
    /*
    PheromoneInf *oPheromoneInf = new PheromoneInf(&vrp);
    SbAS *oSbAS = new SbAS(vrp, oPheromoneInf, vrp.getNumOrders(), 5, 5, 0.95, 5);
    SOD *oCurrentSolution = oSbAS->run();

    qDebug() << oCurrentSolution->getCostSolution();
    ShowRoutes *show = new ShowRoutes(oCurrentSolution, 10, "Solucao");
    show->show();
    */

    //Apply sweep algorithm on instace
    //SweepAlgorithm sweep(vrp);
    //sweep.run();

    //Apply D-Ants
    DAnts oDAnts(vrp, 10);
    SOD *oSolutionBest = oDAnts.run();

    //Get cost solution
    qDebug() << oSolutionBest->getCostSolution() << " in time =" << (float)oTempo.elapsed()/(float)1000 << " sec";
    //vrp.showSolution();

    //Show solution in GUI
    ShowRoutes *show = new ShowRoutes(oSolutionBest, 10, "Solucao");
    show->show();

    return a.exec();

}
