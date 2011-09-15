#ifndef SWEEPALGORITHM_H
#define SWEEPALGORITHM_H
#include "sod.h"

class OrderPolar{

public:
    int iIndex;
    float nAngule;

};

class SweepAlgorithm
{
private:
    SOD oSodInitialSolution;
public:
    SweepAlgorithm(SOD oSolution);
    void run();
    void stepConstrutive();
    void stepLocalSearch();
};

#endif // SWEEPALGORITHM_H
