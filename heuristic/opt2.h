#ifndef OPT2_H
#define OPT2_H
#include "localsearch.h"

class Opt2 : public LocalSearch
{
private:

public:
    Opt2(SOD *opSolution);
    void performeMove();
};

#endif // OPT2_H
