/*#ifndef JOINROUTES_H
#define JOINROUTES_H
#include <QList>
#include "struct/graphm.h"

class Data;
class Moves;
class Moves;
class Route;
class Coordinate;

class JoinRoutes
{
private:

    bool improvement;
    bool hasImprovement;

    int iInsertBestDisp, jInsertBestDisp, kInsertBestDisp, lInserBestDisp, iRouteBestDisp, jRouteBestDisp;
    int p1BestDisp, p2BestDisp, p3BestDisp, p4BestDisp;
    int k1BestDisp, k2BestDisp, k3BestDisp, k4BestDisp, iBestDisp, jBestDisp;
    double bestCost;

    GraphM originalSolution;   //Its stored the original solution
    GraphM currentSolution;    //Its stored the current solution. Its struct store the changes that will be in the solution.
    GraphM bestSolution;       //Its store the bestSolution.

    void doMove(QList<Moves> moves, QList<Route> *routes);
    QList<Moves> tryMove(QList<int> *fullRoute, QList<Route> *routes, QList<int> *indexRoutes, int numJoin, Coordinate* depot);
    QList<Moves> generateMoves(int numRoutesJoin, QList<Route> routes, QList<int> joinPoints);
    QList<int> createJoinPoints(GraphM* newSolution ,QList<Route> routes);
    Moves calcCostPartial(int indexBegin, int indexEnd, QList<int> *fullRoute, Coordinate *depot, int numParts);
    Moves calcCost(QList<int> *fullRoute, Coordinate *depot, int numParts);
    bool comparePolarJoin(int p1, int p2);
    double doDisplacement(int iRoute, int jRoute, int i, int j, GraphM *solution);
    QList<int> getPointsDisplacement(GraphM* solution, int currentRoute, int nextRoute);

    void displacement(GraphM* solution, int indexDepot,int currentRoute, int nextRoute);
    void tryDisplacement(int iRoute, int jRoute, int i, int j, GraphM *solution);
    void doDisplacement(GraphM* solution);




public:

    JoinRoutes(GraphM* routes);
    ~JoinRoutes();
    GraphM join(int numRoutesJoin);    //Its parameter descrive how much routes will be analized for join

};

#endif // JOINROUTES_H
*/
