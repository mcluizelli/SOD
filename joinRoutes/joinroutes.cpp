    /*

  Classe JoinRoutes;
  Desenvolvida por Marcelo Caggiani Luizelli, February, 10, 2011.

  Esta classe destina-se a unir rotas vizinhas e particiona-las novamente com o intuito de diminuir o custo de distancias das rotas;
  Para isto:
    - Selecionar 2 rotas;
    - Juntar os pontos de ambas rotas e construir aplicar algoritmo de varredura (Gillet e Miller) a tais pontos. (Sem considerar restricao de capacidade).
    - Encontrar o melhor ponto que particione a rota de tal forma que minimize a distancia percorrida e factibilize a solucao.

*/
/*
#include "joinroutes.h"
#include "struct/edge.h"
#include "struct/graphm.h"
#include "gui/showroutes.h"
#include "route.h"
#include "moves.h"
#include <iostream>
#include <QDebug>
#include <math.h>

bool compareRelativeValue(Moves m1, Moves m2){
    if(m1.getRelativeValue() < m2.getRelativeValue()) return true;
    else return false;
}

bool JoinRoutes::comparePolarJoin(int c1, int c2){

    if(this->originalSolution.getOrderByIndex(c1)->getAngle() < this->originalSolution.getOrderByIndex(c2)->getAngle()){
        return true;
    }else{
        return false;
    }

}

JoinRoutes::JoinRoutes(GraphM* routes)
{
    this->originalSolution = *routes;
}

JoinRoutes::~JoinRoutes(){

}

GraphM JoinRoutes::join(int numRoutesJoin){


    this->hasImprovement = true;
    QList< Route > routes;
    QList< Moves > betterMoves;
    QList< int > joinPoints;
    int numRoutes = 0;
    this->bestSolution = this->originalSolution.copyOrders2();
    this->bestSolution.setAnguleAllNodes();

    bestSolution.updateInfRoutes();
    bestSolution.updateDemands();
    while(this->hasImprovement){

        //Armazeno em lista as rotas com suas informacoes para melhor manipulacao. Neste ponto defino pontos de juncao entre rotas
       //Precisa-se determinar os pontos de junção.(Fronteira)
       numRoutes = this->bestSolution.getNumRoutes();

       for(int i = 0; i < numRoutes ; i++){
           Route r = Route(bestSolution.getIndexOrdersByRouteNonDirect(i), bestSolution.getCostRouteI(i), bestSolution.getDemandRoute(i), i);
           routes.append(r);
       }

        this->hasImprovement = false;

        joinPoints = createJoinPoints(&bestSolution, routes);

        //Inicia o processo de busca por join

        //Passo num Join Maximo, routes e joinPoints.

        betterMoves = generateMoves(numRoutesJoin, routes, joinPoints);

        doMove(betterMoves, &routes);

        joinPoints.clear();
        betterMoves.clear();
        routes.clear();
        bestSolution.updateInfRoutes();
        bestSolution.updateDemands();

    }

    return this->bestSolution;

}

QList<int> JoinRoutes::createJoinPoints(GraphM* newSolution ,QList<Route> routes){

    int numRoutes = 0;
    int pi = 0, pj = 0;

    int p1i = 0, p2i = 0, p1j = 0, p2j = 0;
    double r1 = 0, r2 = 0, r3 = 0, r4 = 0;
    double angP1i = 0.0, angP2i = 0.0, angP1j = 0.0, angP2j = 0.0;

    QList<int> joinPoints;
    QList<int> aux;
    numRoutes = routes.size();
    newSolution->setAnguleAllNodes();

    for(int i = 0; i + 1 < numRoutes; i++){

        //Seleciona-se os dois pontos extremos de cada rota
        aux = routes.at(i).getOrders();
        p1i = aux.at(1);
        p2i = aux.at(aux.size()-2);
        aux = routes.at(i + 1).getOrders();
        p1j = aux.at(1);
        p2j = aux.at(aux.size()-2);

        //Determino os angulos de cada pontos.
        angP1i = (newSolution->getOrderByIndex(p1i)->getAngle());
        angP2i = (newSolution->getOrderByIndex(p2i)->getAngle());
        angP1j = (newSolution->getOrderByIndex(p1j)->getAngle());
        angP2j = (newSolution->getOrderByIndex(p2j)->getAngle());

        //Calcula-se a razao entre os anglulos para determinar quais dos pontos estao mais proximos e por conseguinte quais que devem ser unidos.
        r1 = ( (angP1i - (angP1j)) > 0) ? (angP1i - (angP1j)) : -1 * ( (angP1i) - (angP1j) );
        r2 = ( (angP2i - (angP2j)) > 0) ? (angP2i - (angP2j)) : -1 * ( (angP2i) - (angP2j) );
        r3 = ( (angP1i - (angP2j)) > 0) ? (angP1i - (angP2j)) : -1 * ( (angP1i) - (angP2j) );
        r4 = ( (angP2i - (angP1j)) > 0) ? (angP2i - (angP1j)) : -1 * ( (angP2i) - (angP1j) );

        //Como pontos de fronteira defino aqueles dois pontos que possuirem a menor razao angular.
        if(r1 < r2 && r1 < r3 && r1 < r4){
            pi = p1i;
            pj = p1j;
        }else if( r2 < r1 && r2 < r3 && r2 < r4 ){
            pi = p2i;
            pj = p2j;
        }else if( r3 < r1 && r3 < r2 && r3 < r4){
            pi = p1i;
            pj = p2j;
        }else if( r4 < r1 && r4 < r2 && r4 < r3){
            pi = p2i;
            pj = p1j;
        }else{
            pi = -1; pj = -1;
                if(p1i == p2i){
                //r1 e r3
                r1 = ( ( (angP1i) - (angP1j)) > 0) ? ( (angP1i) - (angP1j)) : -1 * ( (angP1i) - (angP1j) );
                r2 = ( ( (angP1i) - (angP2j)) > 0) ? ( (angP1i - (angP2j))) : -1 * ( (angP1i) - (angP2j) );
                if( r1 > r2 ) {
                    pj = p1i;
                    pi = p1j;
                }else{
                    pj = p1i;
                    pi = p2j;
                }
            }else{
                //
                r1 = ( ( (angP1j) - (angP1i)) > 0) ? ( (angP1j) - (angP1i)) : -1 * ( (angP1j) - (angP1i) );
                r2 = ( ( (angP1j) - (angP2i)) > 0) ? ( (angP1j - (angP2i))) : -1 * ( (angP1j) - (angP2i) );
                if( r1 < r2){
                    pj = p1j;
                    pi = p1i;
                }else{
                    pj = p1j;
                    pi = p2i;
                }
            }
        }


        joinPoints.append(pi);
        joinPoints.append(pj);
        aux.clear();
    }
    //Ligacao entre o ultimo ponto e o primeiro.
    //joinPoints.append(joinPoints.at(joinPoints.size()-1));
    //joinPoints.append(joinPoints.at(0));

    return joinPoints;
}

QList<Moves> JoinRoutes::generateMoves(int numRoutesJoin, QList<Route> routes, QList<int> joinPoints ){

    /*

    Este metodo realiza os todos os possiveis movimentos dados o numRoutesJoin (Numero maximo de joins) e retorna
    todos aqueles em que apresetam algum ganho.
    Cria N conjuntos de rotas de tamanho <= numJoin, cada subconjunto de rotas iniciando com uma rota diferente do conjunto de rotas.


     */

    QList<Moves> betterMoves;
    QList<Moves> currentMoves;
    QList<int> indexRoutes;
    QList<int> fullRoute;
    QList<int> currentRoute;
    int contJoinPont = 0;
    Coordinate *depot = NULL;
    int numRoutes = 0;

    numRoutes = routes.size();
    depot = this->bestSolution.getOrderByIndex(this->originalSolution.getIndexDepot());

    for(int iRoute = 0; iRoute < numRoutes; iRoute++){

        for(int iNumJoin = 2; iNumJoin <= numRoutesJoin; iNumJoin++){
            //Verifica se é possivel realizar o Join entre as rotas, ou seja, numJoins passado como parametro deve ser maior que o numero de rotas.
            indexRoutes.clear();
            fullRoute.clear();
            currentRoute.clear();

            if(iNumJoin> numRoutes - 1){
                break;
            }

            //Cria Lista de Rotas que serão unidas e armazena na lista indexRoutes.
            for(int i = iRoute, cont = 0; cont < iNumJoin; cont++){
                indexRoutes.append(i);
                if(i == numRoutes - 1) i = 0;
                else i++;
            }

            //Definida Biano subconjunto de rotas que serao unidaas(Join) é necessario construir a grande rota que eh composta da uniao das N rotas.
            //Cria a Grande Rota como uma lista de pontos.
            contJoinPont = iRoute * 2;

            currentRoute = routes.at(indexRoutes.at(0)).getOrders();
            //std::cout << joinPoints.at(contJoinPont % joinPoints.size()) << " / " << currentRoute.at(1) << " / "<< currentRoute.at(currentRoute.size()-2) << std::endl;
            if( joinPoints.at(contJoinPont % joinPoints.size()) == currentRoute.at(1)){
                for(int j = currentRoute.size() - 2; j > 0; j--){   //Indicies de [1,size-2] para nao guardar depositos.
                    fullRoute << currentRoute.at(j);
                }

            }else{
                for(int j = 1; j < currentRoute.size() - 1; j++){   //Indicies de [1,size-2] para nao guardar depositos.
                    fullRoute << currentRoute.at(j);
                }
            }
            contJoinPont++;
            for(int i = 1; i < iNumJoin; i++){
                currentRoute = routes.at(indexRoutes.at(i)).getOrders();
                if( joinPoints.at(contJoinPont % joinPoints.size()) == currentRoute.at(1)){
                    for(int j = 1; j < currentRoute.size() - 1; j++){   //Indicies de [1,size-1] para nao guardar depositos.
                        fullRoute << currentRoute.at(j);
                    }
                }else{
                    for(int j = currentRoute.size() - 2; j > 0; j--){   //Indicies de [1,size-1] para nao guardar depositos.
                        fullRoute << currentRoute.at(j);
                    }
                }
                contJoinPont += 2;

            }


            //Tenta realizar movimento para o conjunto de rotas enumeradas em indexRoutes. Realizar arranjos de tamanho == numRoutesJoin
            currentMoves = tryMove(&fullRoute, &routes, &indexRoutes, iNumJoin, depot);

            for(int i = 0; i < currentMoves.size(); i++){
                //Armazeno todos os movimentos que resultam em melhora, sem considerar demanda.
                if(currentMoves.at(i).getRelativeValue() < 0.999){
                    Moves betterMove;
                    betterMove = currentMoves.at(i);
                    betterMove.setRoutes(indexRoutes);
                    betterMoves.append(betterMove);

                }
            }
        }
    }

    return betterMoves;

}

QList<Moves> JoinRoutes::tryMove(QList<int> *fullRoute, QList<Route> *routes, QList<int> *indexRoutes, int numJoin, Coordinate* depot){
    //Join n Routes and try break it in n routes, where n is whitin [1, n].
    int beginCut = 0, endCut = 0;
    double currentCost = 0.0;
    double fullRouteCost = 0.0;
    Moves newCost;
    QList<int> copyFullRoute;
    QList<Moves> newMoves;
    QList<int> cutPoints;
    QList<int> routesAux;

    //Soma Custo das duas rotas. (Calcula-se para realizar a diminuicao proporcional ao custo original).
    for(int i = 0; i < indexRoutes->size(); i++){
        //std::cout << "Somando custo rota: " << indexRoutes->at(i) << std::endl;
        currentCost += routes->at(indexRoutes->at(i)).getValue();
    }

    //std::cout<<"Custo TESTE: (Rota Unica) " << fullRouteCost << std::endl;
    //std::cout<<"Custo Original das rotas Somado: " << currentCost << std::endl;

    for(int i = 0; i < fullRoute->size(); i++) copyFullRoute.append(fullRoute->at(i));


    //Cria Movimentos de join com diferentes cardinalidades.
    //No momento to criando com cardinalidade = numJoin.

    for(int i = 2; i <= numJoin; i++){

        fullRouteCost = 0.0;
        newCost = calcCost(fullRoute, depot, i);
        if(newCost.getCutPoints().size() != 0){
            for(int j = 0, indexCut = 0; j < fullRoute->size(); ){

                beginCut = newCost.getCutPoints().at(indexCut);
                endCut = newCost.getCutPoints().at(indexCut + 1);
                fullRouteCost += this->originalSolution.getDistance(depot, this->originalSolution.getOrderByIndex(fullRoute->at(j)));

                while(j + 1 < endCut){

                    fullRouteCost += this->originalSolution.getDistance(this->originalSolution.getOrderByIndex(fullRoute->at(j))
                                                                         , this->originalSolution.getOrderByIndex(fullRoute->at(j + 1)));
                    j++;

                }

                j++;
                fullRouteCost += this->originalSolution.getDistance(depot, this->originalSolution.getOrderByIndex( fullRoute->at(endCut - 1) ));
                indexCut += 2;

            }

        newCost.setRelativeValue( (double)fullRouteCost/(double)currentCost );
        for(int j = 0; j <  indexRoutes->size(); j++) routesAux.append(indexRoutes->at(j));
        newCost.setRoutes(routesAux);
        //std::cout << "CutPoints:";
        //for(int i = 0; i < newCost.getCutPoints().size(); i++) std::cout << newCost.getCutPoints().at(i) << " ";
        newCost.setJoinRoutes(copyFullRoute);
        newMoves.append(newCost);
        routesAux.clear();
    }

    }

    return newMoves;

}

Moves JoinRoutes::calcCost(QList<int> *fullRoute, Coordinate *depot, int numParts){
    /*Este metodo calcula a combinacao de separacao de rotas que minimiza a funcao objetivo.
    Para tanto, o algoritmo faz repetidas chamadas ao metodo calcCostPartial variando os
    indicies de inicio e fim de cada rota.
    */
    double cost = (double)RAND_MAX ;
    double costAux = 0.0;
    QList<int> cutPoints;
    Moves aux1, aux2;
    Moves betterMove;
    if(numParts > 1){
        for(int i = 1; i < fullRoute->size() - numParts; i++){
            aux1 = calcCostPartial(0, i, fullRoute, depot, 1);
            aux2 = calcCostPartial(i, fullRoute->size() , fullRoute, depot, numParts-1);

            if(aux1.getValue() + aux2.getValue() < cost){
                cost = aux1.getValue() + aux2.getValue();
                betterMove.setValue(cost);


                betterMove.setCutPoints(aux1.getCutPoints() << aux2.getCutPoints());
                betterMove.setNumJoin(numParts);
                //std::cout << "Cut Points" << std::endl;
                //for(int j = 0; j < betterMove.getCutPoints().size(); j++) std::cout << betterMove.getCutPoints().at(j) << " ";
            }

            costAux = 0.0;

        }
    }

    return betterMove;
}

Moves JoinRoutes::calcCostPartial(int indexBegin, int indexEnd, QList<int> *fullRoute, Coordinate *depot, int numParts){
    double cost = (double)RAND_MAX;
    int indexCurrent = 0;
    Moves betterMove;
    Moves currentMove;
    QList<int> indexCut;
    double distAux = 0.0;
    betterMove.setValue( ((double)RAND_MAX) );

    if(numParts > 1){

       for(int i = indexBegin + 1; i < fullRoute->size(); i++){

            Moves r1, r2;
            r1 = calcCostPartial(indexBegin, i , fullRoute, depot, 1);
            if(numParts - 1 >= 1) r2 = calcCostPartial(i, fullRoute->size(), fullRoute, depot, numParts - 1);

            if(r1.getValue() + r2.getValue() < betterMove.getValue()){
                cost = r1.getValue() + r2.getValue();
                betterMove.setValue(cost);
                betterMove.setCutPoints(r1.getCutPoints() << r2.getCutPoints());
            }
        }

        return betterMove;

    }else if(numParts == 1){
        Moves move;
        QList<int> indexCut;
        double demandTotal = 0.0;
        indexCut.append(indexBegin);
        indexCut.append(indexEnd);
        cost = 0.0;
        indexCurrent = fullRoute->at(indexBegin);
        //std::cout << "Nova rota: " << indexCurrent<< " ";
        distAux = this->originalSolution.getDistance(this->originalSolution.getOrderByIndex(indexCurrent), depot);
        demandTotal += this->originalSolution.getOrderByIndex(indexCurrent)->getDemand();
        //std::cout << "Distancia entre " << indexCurrent << " e " << depot->getIndexOrder() << " = " << distAux << std::endl;
        cost += distAux;
        //std::cout << depot->getIndexOrder()<< " ";
        for(int i = indexBegin + 1; i < indexEnd; i++){
            distAux = this->originalSolution.getDistance(this->originalSolution.getOrderByIndex(indexCurrent), this->originalSolution.getOrderByIndex(fullRoute->at(i)));
            demandTotal += this->originalSolution.getOrderByIndex(fullRoute->at(i))->getDemand();
            cost += distAux;
            //std::cout <<  fullRoute->at(i) << " ";
            //std::cout << "Distancia entre " << indexCurrent << " e " << fullRoute->at(i) << " = " << distAux << std::endl;
            indexCurrent = fullRoute->at(i);
        }
        //std::cout << depot->getIndexOrder()<< " ";
        distAux = this->originalSolution.getDistance(this->originalSolution.getOrderByIndex(fullRoute->at(indexEnd-1)), depot);
        //std::cout << "Distancia entre " << (fullRoute->at(indexEnd-1))<< " e " << depot->getIndexOrder() << " = " << distAux << std::endl;
        cost += distAux;
      //  std::cout << "TOTAL: " << cost << std::endl;
        move.setCutPoints(indexCut);
        move.setValue(cost);
        return move;

    }


}

void JoinRoutes::doMove(QList<Moves> moves, QList<Route> *routes){
    /*
      It method performs moves. Given a ordered list of moves by relative gain, the algorithm try carry out the priority moves according to the best gain.
      If the move is feasible, that is, the sum of demand is less than RouteTime. If is infeasible, try carry out compound move until become feasible. If not possible, try
      other move until there arent moves.
    */

    Moves currentMove;
    int numberRoute = 0;
    int currentRouteAlter = 0, indexRouteAlter = 0;
    int numRotaAux = 0;
    int indexB = 0, infeasibleRoute = 0,currentRoute = 0, nextRoute = 0, indexE = 0;
    int indexBegin = 0, indexEnd = 0, i = 0;
    double demandAux = 0, costAux = 0;
    int depot = 0;
    bool imprime = false;
    bool feasibleMove = false;
    QList<int> indexRoutesInfeasible;
    //GraphM currentSolution = this->bestSolution.copyOrders2();
    depot = this->originalSolution.getIndexDepot();
    qSort(moves.begin(), moves.end(), compareRelativeValue);

    //qDebug() << "Inicido do do Move" << bestSolution.getCost();

    for(int qtdMove = 0; qtdMove < moves.size(); qtdMove++){
        feasibleMove = false;
        //qDebug() << "Novo Tentativa " << qtdMove;
        //currentSolution stores the copy of original solution.

        QList<double> demandTotalByRoute;

        //Get the best moves to try carry out.
        currentMove = moves.at(qtdMove);
        QList<int> fullRoute = currentMove.getJoinRoutes();

        indexBegin = 0, indexEnd = 0;

        for(int qtdRoutes = 0; qtdRoutes < currentMove.getNumJoint(); qtdRoutes++){

            demandAux = 0.0;
            //Get the first and the second cut point. In the list, the pair represent the limits of each route.As the inteval among the pair is closed on first and open on the second element.
            i = currentMove.getCutPoints().at(indexBegin);
            indexEnd = currentMove.getCutPoints().at(indexBegin + 1);
            //The variable i must be less than indexEnd because the interval is closed in second element.
            while(i < indexEnd){
                //Here its carry out the sum of demand of new route.
                demandAux += this->originalSolution.getOrderByIndex(fullRoute.at(i))->getDemand();
                i++;
            }
            //As the cutPoint list is structed in pair of elements, every 2 element represent a begin and an end each route.
            indexBegin += 2;
            demandTotalByRoute.append(demandAux);

        }

        //Verify whith routes are infeasible
        for(int j = 0; j < demandTotalByRoute.size(); j++){

            if(demandTotalByRoute.at(j) > this->originalSolution.getRouteTime()){
                indexRoutesInfeasible.append(currentMove.getIndexRoutes().at(j));
            }

        }

        //If Move is feasible - Only carry out the modifications and restart the process.
        if(indexRoutesInfeasible.size() == 0){

            //std::cout << "Feasible Move" << std::endl;

            //Its strutuc stores the auxiliar solution.
            GraphM auxSolution = this->bestSolution.copyOrders2();
            //Its destroy the routes.
            int dec = 0;
            int rotaIndexMaior = 0;

            for(int j = 0; j < currentMove.getIndexRoutes().size(); j++){
                if(rotaIndexMaior < currentMove.getIndexRoutes().at(j)) rotaIndexMaior = currentMove.getIndexRoutes().at(j);
            }

            for(int j = 0; j < currentMove.getIndexRoutes().size(); j++){

                if(  (currentMove.getIndexRoutes().at(j) == rotaIndexMaior && j == 0) ){
                    auxSolution.removeiRoute(currentMove.getIndexRoutes().at(j));
                    dec = 0;
                }else{
                    if(currentMove.getIndexRoutes().at(j) == rotaIndexMaior){
                        auxSolution.removeiRoute(currentMove.getIndexRoutes().at(j) + dec);
                        dec = 0;
                    }else{
                        auxSolution.removeiRoute(currentMove.getIndexRoutes().at(j) + dec);
                        dec--;
                    }
                }
                //Somente nao decremento caso seja igual ao ultimo elemento
                auxSolution.updateInfRoutes();
            }


            numberRoute = auxSolution.getNumRoutes();

            //Its rebuild the routes in auxSolution.
            for(int qtdRoutes = 0; qtdRoutes <  currentMove.getNumJoint(); qtdRoutes++){

                currentRouteAlter = currentMove.getIndexRoutes().at(qtdRoutes);
                indexRouteAlter = currentMove.getIndexRoutes().indexOf(currentRouteAlter);
                //std::cout << currentRouteAlter << " " << indexRouteAlter << std::endl;
                indexBegin = indexRouteAlter * 2;
                i = currentMove.getCutPoints().at(indexBegin);
                indexEnd = currentMove.getCutPoints().at(indexBegin + 1);
                //for(int i = 0; i < currentMove.getCutPoints().size(); i++) std::cout << currentMove.getCutPoints().at(i) << std::endl;
                //std::cout << "Index Begin: "<<indexBegin << " IndexEnd: "<< indexEnd <<" i " << i<< std::endl;
                auxSolution.addEdge(this->originalSolution.getIndexDepot(), fullRoute.at(i), numberRoute);
                while( (i + 1) < indexEnd){
                    auxSolution.addEdge(fullRoute.at(i),fullRoute.at(i + 1), numberRoute);
                    i++;
                }
                auxSolution.addEdge(fullRoute.at(indexEnd - 1), this->originalSolution.getIndexDepot(), numberRoute);
                //Update the demands in original struct.
                numberRoute++;

                auxSolution.updateInfRoutes();

            }

            auxSolution.setNumRoutes(numberRoute);
            auxSolution.updateInfRoutes();
            auxSolution.updateDemands();
            indexRoutesInfeasible.clear();
            auxSolution.updateInfRoutes();
            this->bestSolution = auxSolution;
            this->hasImprovement = true;
            //auxSolution.~GraphM();
            return ;

        }else{
            //Its strutuc stores the auxiliar solution.
            numberRoute = 0;
            GraphM auxSolution = bestSolution.copyOrders2();
            //ShowRoutes *sr7 = new ShowRoutes(auxSolution, 10, "original");
            //sr7->show();

            //Its destroy the routes.
            int dec = 0;
            int rotaIndexMaior = 0;

            for(int j = 0; j < currentMove.getIndexRoutes().size(); j++){
                if(rotaIndexMaior < currentMove.getIndexRoutes().at(j)) rotaIndexMaior = currentMove.getIndexRoutes().at(j);
            }


            for(int j = 0; j < currentMove.getIndexRoutes().size(); j++){

                if(  (currentMove.getIndexRoutes().at(j) == rotaIndexMaior && j == 0) ){
                    auxSolution.removeiRoute(currentMove.getIndexRoutes().at(j));
                    dec = 0;
                }else{
                    if(currentMove.getIndexRoutes().at(j) == rotaIndexMaior){
                        auxSolution.removeiRoute(currentMove.getIndexRoutes().at(j) + dec);
                        dec = 0;
                    }else{
                        auxSolution.removeiRoute(currentMove.getIndexRoutes().at(j) + dec);
                        dec--;
                    }
                }
                //Somente nao decremento caso seja igual ao ultimo elemento
                auxSolution.updateInfRoutes();
            }

            if(imprime){
                ShowRoutes *sr = new ShowRoutes(auxSolution, 10, "teste");
                sr->show();
            }

            numberRoute = auxSolution.getNumRoutes();

            //Its rebuild the routes in auxSolution.
            for(int qtdRoutes = 0; qtdRoutes <  currentMove.getNumJoint(); qtdRoutes++){

                currentRouteAlter = currentMove.getIndexRoutes().at(qtdRoutes);
                indexRouteAlter = currentMove.getIndexRoutes().indexOf(currentRouteAlter);
                //std::cout << currentRouteAlter << " " << indexRouteAlter << std::endl;
                indexBegin = indexRouteAlter * 2;
                i = currentMove.getCutPoints().at(indexBegin);
                indexEnd = currentMove.getCutPoints().at(indexBegin + 1);
                //for(int i = 0; i < currentMove.getCutPoints().size(); i++) std::cout << currentMove.getCutPoints().at(i) << std::endl;
                //std::cout << "Index Begin: "<<indexBegin << " IndexEnd: "<< indexEnd <<" i " << i<< std::endl;
                auxSolution.addEdge(this->originalSolution.getIndexDepot(), fullRoute.at(i), numberRoute);
                while( (i + 1) < indexEnd){
                    auxSolution.addEdge(fullRoute.at(i),fullRoute.at(i + 1), numberRoute);
                    i++;
                }
                auxSolution.addEdge(fullRoute.at(indexEnd - 1), this->originalSolution.getIndexDepot(), numberRoute);
                //Update the demands in original struct.
                numberRoute++;

                auxSolution.updateInfRoutes();

            }

            auxSolution.setNumRoutes(numberRoute);
            auxSolution.updateInfRoutes();            
            indexRoutesInfeasible.clear();

            for(int r = 0; r < auxSolution.getNumRoutes(); r++){
                if(auxSolution.getDemandRoute(r) > auxSolution.getRouteTime()) indexRoutesInfeasible.append(r);
            }


            //Here, the solution try turn feasible.

            for(int i = 0; i < indexRoutesInfeasible.size(); i++){

                //Tyr feasible the route defined on indexRoutesInfeasible.
                //In this step, the algorithm try moves the more distance point to other routes adjacents.
                //Here, the moviment is performed in clockwise direction.

                int excesso = 0;
                int contingente = 0;
                for(int qtdR = 0; qtdR < auxSolution.getNumRoutes(); qtdR++){
                    if( auxSolution.getDemandRoute(qtdR) <= auxSolution.getRouteTime()) excesso += (auxSolution.getRouteTime() - auxSolution.getDemandRoute(qtdR) );
                    else contingente = ( auxSolution.getDemandRoute(qtdR) - auxSolution.getRouteTime());
                }
                //auxSolution.updateInfRoutes();
                if(contingente >= excesso) break;
                indexB = currentMove.getIndexRoutes().indexOf(indexRoutesInfeasible.at(i));
                infeasibleRoute = indexRoutesInfeasible.at(i);
                currentRoute = infeasibleRoute;
                nextRoute = (infeasibleRoute + 1) % auxSolution.getNumRoutes();
                indexE = nextRoute;

                //Estrutura de controle que controlar os movimentos compostos.
                for(int numCiclo = 0; numCiclo < 2; numCiclo++){

                    while( nextRoute != infeasibleRoute ){

                        //qDebug() << " Rotas em questao: " << currentRoute << " e " << nextRoute;

                        if(auxSolution.getDemandRoute(currentRoute) > auxSolution.getRouteTime()){
                            //qDebug() << "Num de rotas" << auxSolution.getNumRoutes();
                            displacement(&auxSolution, depot, currentRoute, nextRoute);
                            //qDebug() << auxSolution.getDemandRoute(currentRoute) << " " << auxSolution.getDemandRoute(nextRoute);
                            if(auxSolution.getDemandRoute(nextRoute) > this->originalSolution.getRouteTime()){
                                currentRoute = nextRoute;
                                nextRoute = (currentRoute + 1) % auxSolution.getNumRoutes();
                            }else{
                                currentRoute = nextRoute;
                                nextRoute = (currentRoute + 1) % auxSolution.getNumRoutes();
                                //auxSolution.showDemandsAllRoutes();
                                //auxSolution.showAllRoutes();
                                feasibleMove = true;

                                //qDebug() << "Custo Nova: " << auxSolution.getCost();
                                break;
                            }

                        }else{
                            currentRoute = nextRoute;
                            nextRoute = (currentRoute + 1) % auxSolution.getNumRoutes();
                        }
                    }
                    infeasibleRoute = (infeasibleRoute + (auxSolution.getNumRoutes() - 1) ) % auxSolution.getNumRoutes();
                    if(feasibleMove) break;
                    //qDebug() << nextRoute;
                    nextRoute = (nextRoute == 0) ? auxSolution.getNumRoutes() -1  : nextRoute - 1 ;
                    //qDebug() << nextRoute;

                }
                //Se uma das rotas não foi possivel factbilizar, interrompe-se o mivmento.
                if (!feasibleMove) {
                    feasibleMove = false;
                    break;
                }
                //Verifica-se se todas as rotas foram factibilizadas.
                if( (i + 1) == indexRoutesInfeasible.size()){

                    feasibleMove = true;
                    for(int j = 0; j < auxSolution.getNumRoutes(); j++){
                        if(auxSolution.getDemandRoute(j) > auxSolution.getRouteTime()){
                            feasibleMove = false;
                        }
                    }

                    if( !feasibleMove){
                        qDebug() << "Nao foi possivel factibilizar o movimento.";
                        break;

                    }else{

                        //Somente aqui armazeno a solucao na solucao original, pois aqui ha a garantia que todas as rotas envolvidas no movimento foram factibilizadas.
                        //qDebug() << auxSolution.getCost() << " < " << this->bestSolution.getCost();
                        if( auxSolution.getCost() < this->bestSolution.getCost() && i == indexRoutesInfeasible.size() - 1 ){

                            this->hasImprovement = true;
                            //qDebug() << "Movimento factivel " << auxSolution.getCost();

                            this->bestSolution = auxSolution;
                            return;

                        }else{
                            //qDebug() << "Movimento factivel mas com custo superior" << auxSolution.getCost();
                            this->hasImprovement = false;
                        }

                    }

                }

            }

        }

        indexRoutesInfeasible.clear();

    }

    this->hasImprovement = false;

}

QList<int> JoinRoutes::getPointsDisplacement(GraphM* solution, int currentRoute, int nextRoute){

    int p1i, p2i, p1j, p2j, pi, pj;
    double r1, r2, r3, r4, angP1i, angP1j, angP2i, angP2j, dist1, dist2, dist3, dist4;
    QList<int> pointsNextRoute = solution->getIndexOrdersByRouteNonDirect(nextRoute);
    QList<int> pointsCurrentRoute = solution->getIndexOrdersByRouteNonDirect(currentRoute);
    QList<int> pointDisplacement;
    p1i = pointsNextRoute.at(1);
    p2i = pointsNextRoute.at(pointsNextRoute.size()-2);

    p1j = pointsCurrentRoute.at(1);
    p2j = pointsCurrentRoute.at(pointsCurrentRoute.size()-2);
    //std::cout << "Candidatos ao deslocamento: " << p1i << " " << p2i << " " << p1j << " " << p2j << std::endl;


        angP1i = (solution->getOrderByIndex(p1i)->getAngle());
        angP2i = (solution->getOrderByIndex(p2i)->getAngle());
        angP1j = (solution->getOrderByIndex(p1j)->getAngle());
        angP2j = (solution->getOrderByIndex(p2j)->getAngle());

        r1 = ( (angP1i - (angP1j)) > 0) ? (angP1i - (angP1j)) : -1 * ( (angP1i) - (angP1j) );
        r2 = ( (angP2i - (angP2j)) > 0) ? (angP2i - (angP2j)) : -1 * ( (angP2i) - (angP2j) );
        r3 = ( (angP1i - (angP2j)) > 0) ? (angP1i - (angP2j)) : -1 * ( (angP1i) - (angP2j) );
        r4 = ( (angP2i - (angP1j)) > 0) ? (angP2i - (angP1j)) : -1 * ( (angP2i) - (angP1j) );

        if(r1 < r2 && r1 < r3 && r1 < r4){
            pi = p1i;
            pj = p1j;
        }else if( r2 < r1 && r2 < r3 && r2 < r4 ){
            pi = p2i;
            pj = p2j;
        }else if( r3 < r1 && r3 < r2 && r3 < r4){
            pi = p1i;
            pj = p2j;
        }else if( r4 < r1 && r4 < r2 && r4 < r3){
            pi = p2i;
            pj = p1j;
        }else{
            pi = -1; pj = -1;
                if(p1i == p2i){
                r1 = ( ( (angP1i) - (angP1j)) > 0) ? ( (angP1i) - (angP1j)) : -1 * ( (angP1i) - (angP1j) );
                r2 = ( ( (angP1i) - (angP2j)) > 0) ? ( (angP1i - (angP2j))) : -1 * ( (angP1i) - (angP2j) );
                if( r1 > r2 ) {
                    pj = p1i;
                    pi = p1j;
                }else{
                    pj = p1i;
                    pi = p2j;
                }
            }else{
                r1 = ( ( (angP1j) - (angP1i)) > 0) ? ( (angP1j) - (angP1i)) : -1 * ( (angP1j) - (angP1i) );
                r2 = ( ( (angP1j) - (angP2i)) > 0) ? ( (angP1j - (angP2i))) : -1 * ( (angP1j) - (angP2i) );
                if( r1 < r2){
                    pj = p1j;
                    pi = p1i;
                }else{
                    pj = p1j;
                    pi = p2i;
                }
            }
        }

        //std::cout<<pj << " " << pi << std::endl;
        pointDisplacement.append(pj);
        pointDisplacement.append(pi);


        dist1 = solution->getDistance(solution->getOrderByIndex(p1i), solution->getOrderByIndex(p1j));
        dist2 = solution->getDistance(solution->getOrderByIndex(p1i), solution->getOrderByIndex(p2j));
        dist3 = solution->getDistance(solution->getOrderByIndex(p2i), solution->getOrderByIndex(p1j));
        dist4 = solution->getDistance(solution->getOrderByIndex(p2i), solution->getOrderByIndex(p2j));

        if(dist1 <= dist2 && dist1<= dist3 && dist1<= dist4){
            pi = p1i;
            pj = p1j;
        }else if(dist2 <= dist1 && dist2<= dist3 && dist2<= dist4){
            pi = p1i;
            pj = p2j;
        }else if(dist3 <= dist1 && dist3 <= dist2 && dist3 <= dist4){
            pi = p2i;
            pj = p1j;
        }else if(dist4 <= dist1 && dist4 <= dist2 && dist4 <= dist3){
            pi = p2i;
            pj = p2j;
        }
        //std::cout<<pj << " " << pi << std::endl;

        pointDisplacement.append(pj);
        pointDisplacement.append(pi);


    return pointDisplacement;

}

void JoinRoutes::tryDisplacement(int iRoute, int jRoute, int i, int j, GraphM *solution){
    //O ponto "i" eh o ponto que eu removo da rota iRoute. Para isto faćo o seguinte:
    int indexDepot = solution->getIndexDepot();
    QList<Edge> iInOutEdge = solution->inOutEdge(i);
    QList<Edge> jInOutEdge = solution->inOutEdge(j);
    Coordinate *k2Aux, *k4Aux, *iAux, *k1Aux, *k3Aux;
    double newCost;
    int p1 , p2, p3, p4; //Ponto para i
    int k1 = -1, k2 = -1, k3 = -1, k4 = -1; //Ponto para j
    int iInsert = -1;   //Flags para reconstruir a solucao inicial
    int jInsert = -1;   //Flags para reconstruir a solucao inicial
    int kInsert = -1;
    if(iInOutEdge.size() == 2 && jInOutEdge.size() == 2){
        //Realiza a primeira mudanca - Remocao do ponto da iRoute
        p1 = iInOutEdge.at(0).getP1();
        p2 = iInOutEdge.at(0).getP2();
        p3 = iInOutEdge.at(1).getP1();
        p4 = iInOutEdge.at(1).getP2();

        //std::cout<<"Arestas rota i : "<<p1<<" "<<p2<<" "<<p3<<" "<<p4<<std::endl;

        solution->removeEdge(p1, p2);
        solution->removeEdge(p3, p4);

        if(p1 == p3){
            solution->addEdge(p2, p4, iRoute);
            iInsert = 1;
        }else if(p1 == p4){
            solution->addEdge(p2, p3, iRoute);
            iInsert = 2;
        }else if(p2 == p3){
            solution->addEdge(p1, p4, iRoute);
            iInsert = 3;
        }else if(p2 == p4){
            solution->addEdge(p1, p3, iRoute);
            iInsert = 4;
        }
        //Fim do procedimento de Remocao do ponto i

        //Inicio da insercao na outra jRoute
        k1 = jInOutEdge.at(0).getP1();
        k2 = jInOutEdge.at(0).getP2();
        k3 = jInOutEdge.at(1).getP1();
        k4 = jInOutEdge.at(1).getP2();
        //std::cout<<"Arestas rota j : "<<k1<<" "<<k2<<" "<<k3<<" "<<k4<<std::endl;
        //std::cout<<"Valor i e j: "<<i<< "  "<< j <<std::endl;
        if(k1 == k3 && k1 != indexDepot){
            jInsert = 1;
            k2Aux = solution->getOrder(k2);
            k4Aux = solution->getOrder(k4);
            iAux = solution->getOrder(i);

            solution->addEdge(i, j, jRoute);
            if(solution->getDistance(k2Aux, iAux) < solution->getDistance(k4Aux, iAux)){
                //Verifica-se se esta aresta jah existe, caso exista nao a adiciono
                if( solution->getValueMatrix(k2, i) != -1 ){
                    solution->removeEdge(k1, k2);
                    kInsert = 1;
                    lInserBestDisp = 1;
                }else{
                    solution->addEdge(k2, i, jRoute);
                    solution->removeEdge(k1, k2);
                    kInsert = 1;
                    lInserBestDisp = 2;
                }

            }else{
                //Verica-se se esta aresta jah existe.
                if( solution->getValueMatrix(k4, i) != -1){
                    solution->removeEdge(k3, k4);
                    kInsert = 2;
                    lInserBestDisp = 1;
                }else{
                    solution->addEdge(k4, i, jRoute);
                    solution->removeEdge(k3, k4);
                    kInsert = 2;
                    lInserBestDisp = 2;
                }
            }
        }else if(k1 == k4 && k1 != indexDepot){
            jInsert = 2;
            k2Aux = solution->getOrder(k2);
            k3Aux = solution->getOrder(k3);
            iAux = solution->getOrder(i);
            solution->addEdge(i, j, jRoute);
            if(solution->getDistance(k2Aux, iAux) < solution->getDistance(k3Aux, iAux)){
                //Verifica se existe a aresta.
                if(solution->getValueMatrix(k2, i) != -1){
                    solution->removeEdge(k1, k2);
                    kInsert = 1;
                    lInserBestDisp = 1;
                }else{
                    solution->addEdge(k2, i, jRoute);
                    solution->removeEdge(k1, k2);
                    kInsert = 1;
                    lInserBestDisp = 2;
                }

            }else{
                //std::cout<<"teste :"<<k3<<","<<i<<"|"<<solution->getValueMatrix(k3, i)<<" "<<solution->getValueMatrix(i, k3)<<std::endl;
                //Verifica se existe a aresta.
                if(solution->getValueMatrix(k3, i) != -1){
                    solution->removeEdge(k3, k4);
                    kInsert = 2;
                    lInserBestDisp = 1;
                }else{
                    solution->addEdge(k3, i, jRoute);
                    solution->removeEdge(k3, k4);
                    kInsert = 2;
                    lInserBestDisp = 2;
                }

            }
        }else if(k2 == k3 && k2 != indexDepot){
            jInsert = 3;
            k1Aux = solution->getOrder(k1);
            k4Aux = solution->getOrder(k4);
            iAux = solution->getOrder(i);

            solution->addEdge(i, j, jRoute);
            if(solution->getDistance(k1Aux, iAux) < solution->getDistance(k4Aux, iAux)){
                if(solution->getValueMatrix(k1, i) != -1){
                    solution->removeEdge(k1, k2);
                    kInsert = 1;
                    lInserBestDisp = 1;
                }else{
                    solution->addEdge(k1, i, jRoute);
                    solution->removeEdge(k1, k2);
                    kInsert = 1;
                    lInserBestDisp = 2;
                }

            }else{
                if(solution->getValueMatrix(k4, i) != -1){
                    solution->removeEdge(k3, k4);
                    kInsert = 2;
                    lInserBestDisp = 1;
                }else{
                    solution->addEdge(k4, i, jRoute);
                    solution->removeEdge(k3, k4);
                    kInsert = 2;
                    lInserBestDisp = 2;
                }

            }
        }else if(k2 == k4 && k2 != indexDepot){
            jInsert = 4;
            k1Aux = solution->getOrder(k1);
            k3Aux = solution->getOrder(k3);
            iAux = solution->getOrder(i);

            solution->addEdge(i, j, jRoute);
            if(solution->getDistance(k1Aux, iAux) < solution->getDistance(k3Aux, iAux)){
                if(solution->getValueMatrix(k1, i) != -1){
                    solution->removeEdge(k1, k2);
                    kInsert = 1;
                    lInserBestDisp = 1;
                }else{
                    solution->addEdge(k1, i, jRoute);
                    solution->removeEdge(k1, k2);
                    kInsert = 1;
                    lInserBestDisp = 2;
                }

            }else{
                if(solution->getValueMatrix(k3, i) != -1){
                    solution->removeEdge(k3, k4);
                    kInsert = 2;
                    lInserBestDisp = 1;
                }else{
                    solution->addEdge(k3, i, jRoute);
                    solution->removeEdge(k3, k4);
                    kInsert = 2;
                    lInserBestDisp = 2;
                }

            }
        }

        //Fim da insercao na segunda rota...
        //Avaliacao do custo; // Fazer para avaliar o custo das rotas somente.
        newCost = solution->getCost();
        //std::cout<<newCost<< " "<<bestCost<<std::endl;

        if(newCost < bestCost){
            //Salva inf.
            iInsertBestDisp = iInsert;
            jInsertBestDisp = jInsert;
            kInsertBestDisp = kInsert;
            iRouteBestDisp = iRoute;
            jRouteBestDisp = jRoute;
            p1BestDisp = p1;
            p2BestDisp = p2;
            p3BestDisp = p3;
            p4BestDisp = p4;
            k1BestDisp = k1;
            k2BestDisp = k2;
            k3BestDisp = k3;
            k4BestDisp = k4;
            iBestDisp = i;
            jBestDisp = j;
            bestCost = newCost;

        }
        //Desfazer alteracoes
        //std::cout<<"Modos: "<<iInsert<<" "<<jInsert<<  " "<<kInsert<<" "<<lInserBestDisp<<std::endl;

        //Rota 2
        if(jInsert == 1){
            solution->removeEdge(i, j);
            if(kInsert == 1){
                if(lInserBestDisp == 1){
                    solution->addEdge(k1, k2, jRoute);
                }else{
                    solution->removeEdge(k2, i);
                    solution->addEdge(k1, k2, jRoute);
                }
            }else if(kInsert == 2){
                if(lInserBestDisp == 1){
                    solution->addEdge(k3, k4, jRoute);
                }else{
                    solution->removeEdge(k4, i);
                    solution->addEdge(k3, k4, jRoute);
                }
            }
        }else if(jInsert == 2){
            solution->removeEdge(i, j);
            if(kInsert == 1){
                if(lInserBestDisp == 1){
                    solution->addEdge(k1, k2, jRoute);
                }else{
                    solution->removeEdge(k2, i);
                    solution->addEdge(k1, k2, jRoute);
                }
            }else if(kInsert == 2){
                if(lInserBestDisp == 1){
                    solution->addEdge(k3, k4, jRoute);
                }else{
                    solution->removeEdge(k3, i);
                    solution->addEdge(k3, k4, jRoute);
                }
            }
        }else if(jInsert == 3){
            solution->removeEdge(i ,j);
            if(kInsert == 1){
                if(lInserBestDisp == 1){
                    solution->addEdge(k1, k2, jRoute);
                }else{
                    solution->removeEdge(k1, i);
                    solution->addEdge(k1, k2, jRoute);
                }
            }else if(kInsert == 2){
                if(lInserBestDisp == 1){
                    solution->addEdge(k3, k4, jRoute);
                }else{
                    solution->removeEdge(k4, i);
                    solution->addEdge(k3, k4, jRoute);
                }
            }
        }else if(jInsert == 4){
            solution->removeEdge(i ,j);
            if(kInsert == 1){
                if(lInserBestDisp == 1){
                    solution->addEdge(k1, k2, jRoute);
                }else{
                    solution->removeEdge(k1, i);
                    solution->addEdge(k1, k2, jRoute);
                }
            }else if(kInsert == 2){
                if(lInserBestDisp == 1){
                    solution->addEdge(k3, k4, jRoute);
                }else{
                    solution->removeEdge(k3, i);
                    solution->addEdge(k3, k4, jRoute);
                }

            }
        }

        //Rota 1 - Reinserir ponto e remover arestas colocadas
        solution->addEdge(p1, p2, iRoute);
        solution->addEdge(p3, p4, iRoute);
        if(iInsert == 1){
            solution->removeEdge(p2, p4);
        }else if(iInsert == 2){
            solution->removeEdge(p2, p3);
        }else if(iInsert == 3){
            solution->removeEdge(p1, p4);
        }else if(iInsert == 4){
            solution->removeEdge(p1, p3);
        }

    }

}

void JoinRoutes::displacement(GraphM* solution, int indexDepot, int currentRoute, int nextRoute){



    double demandaMax = solution->getRouteTime();
    double demandaAux;
    //std::cout << solution->getValueMatrix(45, 50) << " " << solution->getValueMatrix(50, 45) << std::endl;
    while(solution->getDemandRoute(currentRoute) > demandaMax){
        this->bestCost = (double)RAND_MAX;

        QList<int> pointsDisplacement = getPointsDisplacement(solution, currentRoute, nextRoute);
        QList<int> pontosRotaProxima = solution->getIndexOrdersByRouteNonDirect(nextRoute);
        QList<int> pontosRotaCorrente = solution->getIndexOrdersByRouteNonDirect(currentRoute);
        //std::cout<<"Pontos current Route" << std::endl;
        //for(int i = 0; i < pontosRotaCorrente.size(); i++) std::cout << pontosRotaCorrente.at(i) << " ";

        //std::cout<<"Pontos next Route" << std::endl;
        //for(int i = 0; i < pontosRotaProxima.size(); i++) std::cout << pontosRotaProxima.at(i) << " ";

        this->bestCost = (double)RAND_MAX;

        for(int iElemento = 1; iElemento < pontosRotaCorrente.size() -1;  iElemento++){
        //for(int iElemento = 0 ; iElemento < pointsDisplacement.size() ; iElemento = iElemento + 2){

            int iIndexElemento = pontosRotaCorrente.at(iElemento);

            for(int jElemento = 1; jElemento < pontosRotaProxima.size() - 1 ; jElemento++){

                //int iIndexElemento = pointsDisplacement.at(iElemento);
                int jIndexElemento = pontosRotaProxima.at(jElemento);

                if( iIndexElemento != indexDepot && jIndexElemento != indexDepot && iIndexElemento != jIndexElemento){
                    //std::cout << "Tenta: " << pontosRotaCorrente.at(iElemento) << "  " << pontosRotaProxima.at(jElemento) << std::endl;
                    //tryDisplacement(currentRoute, nextRoute, pointsDisplacement.at(iElemento), pontosRotaProxima.at(jElemento), solution);
                    tryDisplacement(currentRoute, nextRoute, pontosRotaCorrente.at(iElemento), pontosRotaProxima.at(jElemento), solution);
                }

             }

        }
        //solution->verifyDemandsRoutes();

        doDisplacement(solution);
        demandaAux = solution->getOrderByIndex(iBestDisp)->getDemand();
        solution->setDemandRoute(nextRoute, (solution->getDemandRoute(nextRoute) + demandaAux));
        solution->setDemandRoute(currentRoute, (solution->getDemandRoute(currentRoute) - demandaAux));
        //solution->verifyDemandsRoutes();
        //std::cout<<"Demanda Atual: " << solution->getDemandRoute(currentRoute) << std::endl;
        //std::cout<<"Demanda Proxima " << solution->getDemandRoute(nextRoute) << std::endl;

    }

}

void JoinRoutes::doDisplacement(GraphM* solution){
    //std::cout << "Remove: "<< p1BestDisp << " " << p2BestDisp << std::endl;
    //std::cout << "Remove: "<< p3BestDisp << " " << p4BestDisp << std::endl;
    solution->removeEdge(p1BestDisp, p2BestDisp);
        solution->removeEdge(p3BestDisp, p4BestDisp);

        if(iInsertBestDisp == 1){
            solution->addEdge(p2BestDisp, p4BestDisp, iRouteBestDisp);
        }else if(iInsertBestDisp == 2){
            solution->addEdge(p2BestDisp, p3BestDisp, iRouteBestDisp);
        }else if(iInsertBestDisp == 3){
            solution->addEdge(p1BestDisp, p4BestDisp, iRouteBestDisp);
        }else if(iInsertBestDisp == 4){
            solution->addEdge(p1BestDisp, p3BestDisp, iRouteBestDisp);
        }

        if(jInsertBestDisp == 1){
            solution->addEdge(iBestDisp, jBestDisp, jRouteBestDisp);
            if(kInsertBestDisp == 1){
                solution->addEdge(k2BestDisp, iBestDisp, jRouteBestDisp);
                solution->removeEdge(k1BestDisp, k2BestDisp);
            }else if(kInsertBestDisp == 2){
                solution->addEdge(k4BestDisp, iBestDisp, jRouteBestDisp);
                solution->removeEdge(k3BestDisp, k4BestDisp);
            }
        }else if(jInsertBestDisp == 2){
            solution->addEdge(iBestDisp, jBestDisp, jRouteBestDisp);
            if(kInsertBestDisp == 1){
                solution->addEdge(k2BestDisp, iBestDisp, jRouteBestDisp);
                solution->removeEdge(k1BestDisp, k2BestDisp);
            }else if(kInsertBestDisp == 2){
                solution->addEdge(k3BestDisp, iBestDisp, jRouteBestDisp);
                solution->removeEdge(k3BestDisp, k4BestDisp);
            }
        }else if(jInsertBestDisp == 3){
            solution->addEdge(iBestDisp ,jBestDisp, jRouteBestDisp);
            if(kInsertBestDisp == 1){
                solution->addEdge(k1BestDisp, iBestDisp, jRouteBestDisp);
                solution->removeEdge(k1BestDisp, k2BestDisp);
            }else if(kInsertBestDisp == 2){
                solution->addEdge(k4BestDisp, iBestDisp, jRouteBestDisp);
                solution->removeEdge(k3BestDisp, k4BestDisp);
            }
        }else if(jInsertBestDisp == 4){
            solution->addEdge(iBestDisp, jBestDisp, jRouteBestDisp);
            if(kInsertBestDisp == 1){
                solution->addEdge(k1BestDisp, iBestDisp, jRouteBestDisp);
                solution->removeEdge(k1BestDisp, k2BestDisp);
            }else{
                solution->addEdge(k3BestDisp, iBestDisp, jRouteBestDisp);
                solution->removeEdge(k3BestDisp, k4BestDisp);
            }
        }
}

