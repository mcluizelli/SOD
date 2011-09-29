#ifndef PHEROMONEINF_H
#define PHEROMONEINF_H
#include "sod.h"

class PheromoneInf
{
private:
    int     iNumOrders;
    float **npPheromoneConcentration;
public:
    PheromoneInf(SOD *oSODSolution);
    float getPheromoneArc(int iIArc, int iJArc);
    void  setPheromoneArc(int iIArc, int iJArc, float nValue);
    void showInf();
};

#endif // PHEROMONEINF_H
