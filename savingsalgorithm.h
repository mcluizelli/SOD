#ifndef SAVINGSALGORITHM_H
#define SAVINGSALGORITHM_H
#include "sod.h"
#include "sbas.h"

class SavingsAlgorithm
{
private:
    SOD oSodInitialSolution;
public:
    SavingsAlgorithm(SOD opSolution);
    QList<SavingAS> buildSavingList(Depot *oDepot);
    SOD run();
};

#endif // SAVINGSALGORITHM_H
