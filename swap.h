#ifndef SWAP_H
#define SWAP_H
#include "localsearch.h"

class Swap : public LocalSearch
{
public:
    Swap(SOD *opSolution);
    void performeMove();
};

#endif // SWAP_H
