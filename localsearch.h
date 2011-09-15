#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include "sod.h"
class LocalSearch
{

protected:

    SOD *opSolution;

private:

    virtual void performeMove() = 0;
  //  virtual void search() = 0;
  //  virtual void firstImprovement() = 0;
  //  virtual void bestImprovement() = 0;

};

#endif // LOCALSEARCH_H
