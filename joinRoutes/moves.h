/*#ifndef MOVES_H
#define MOVES_H
#include "simplemove.h"
#include <QList>

class Moves
{
private:
    double relativeValue;       //Valor em relacao a reducao obtida com os movimentos (Proporcional ao tamanho)
    double value;
    int numJoin;
    QList<double> demandRoute;  //Indices correspondem as rotas indexadas em indexRoutes.
    QList<SimpleMove> moves;    //Movimento composto que factibilizam a solucao, se necessario.
    QList<int> indexRoutes;
    QList<int> joinRoutes;
    QList<int> joinPoints;
    QList<int> cutPoints;
    bool hasCompoundMoviment;   //Esta variavel indica se há movimento composto a ser realizado.

public:
    Moves();
    Moves(double value,double relativeValue,  QList<SimpleMove> moves);
    void setRoutes(QList<int> indexRoutes);
    void setDemandRoute(QList<double> demand);
    void setNumJoin(int numJoin);
    void setJoinPoints(QList<int> joinPoints);
    void setJoinRoutes(QList<int> joinRoutes);
    void setHasCompoundMoviment(bool value);
    void setValue(double value);
    void setRelativeValue(double relativeValue) ;
    bool getHasCompoundMoviment();
    void setCutPoints(QList<int> cutPoints);
    double getValue() const;
    double getRelativeValue() const;
    int getNumJoint();
    QList<int> getIndexRoutes()const;
    QList<double> getDemandRoutes();
    QList<int> getCutPoints() const;
    QList<int> getJoinRoutes();
    QList<SimpleMove> getSimpleMoves();
};

#endif // MOVES_H
*/
