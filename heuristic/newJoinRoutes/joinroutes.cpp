#include "joinroutes.h"

JoinRoutes::JoinRoutes()
{
}

void JoinRoutes::run(){

    bool bFeasible    = false;
    bool bTryFeasible = false;
    bool bHasImprove  = true;

    while(bHasImprove){

        bHasImprove = false;
        //Create possible moves based on iNumJoin
        QList<> oListMoves = createMoves(iNumJoin);
        //Sort oListMoves in decrease order (Based on function)
        qSort(oListMoves.begin(), oListmoves.end(), compareFuncition);
        //For each moves, try perform it until reach it.
        for(int iContMove = 0; iContMove < oListMoves.size(); iContMove++){

            Type oMove = oListMoves.at(iContMove);
            //Try reach move.
            bFeasible = tryMove(oMove);
            //If the move is feasible, so its performed. Else, its activated a procedure that try turn feasible the move.
            if(bFeasible){

                performMove(oMove);

            }else{

                bTryFeasible = tryFeasible(oMove);
                if(bTryFeasible){

                }

            }

        }

        if(!bHasImprove) break;

    }

}
