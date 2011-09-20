/*
###################################################################################################################
    This class is a implementation of Saving based Ant System for VRP. This method consist mainly in three steps.
 The first step consist in a construtive method. A second step is a local search aplication. A third step consist
 on pheromone update.
 For the construtive step, we consider the classical saving heuristic proposed by Clarke and Wright (1964).
    @author Marcelo Caggiani Luizelli
    @date   16/08/2011
###################################################################################################################
*/
#ifndef SBAS_H
#define SBAS_H
#include "sod.h"
#include "pheromoneinf.h"
/*
    This class definided in class describe a struct to storage all information about the saving lists.
*/
class SavingAS{
public:
    int   iOrder1;
    int   iOrder2;
    float nValue;
};

class AtractivenessItem{
public:
    SavingAS iItem;
    int      iIndex;
};

class SbAS
{
private:
    SOD             oSodInitialSolution;
    QList<SOD*>     oSodElitistListSolution;
    SOD*            oSODBestSolution;
    //This variables represents the relavative influence of the saving and the pheromone trails.
    float           nBeta, nAlpha, nRo;
    PheromoneInf    *oPheromoneConcentration;
    int             iKNeighbor, iNumTotalAnts, iNumSulutionElitist;

    QList<SavingAS> buildSavingList(Depot *oDepot);
    void            saveSolution(SOD *oSolution);

public:
    SbAS(SOD oSodInitialSolution, PheromoneInf *oPheromoneInf, int iNumTotalAnts, float nAlpha, float nBeta, float nRo, int iNumSolutionElitist);
    SOD* run();
    //In this step is aplied a saving algorith for the Ant System.
    SOD* stepConstrutive();
    //In this step is aplied two heuristics. A swap and a 2opt.
    void stepLocalSerch(SOD *oSolution);
    //Here is update the pheromone trail.
    void stepUpdatePheromone();
    //Return atractiveness list value.
    int returnAtractivenessList(QList<SavingAS> *oListSaving, Depot *oDepotAux);
};

#endif // SBAS_H
