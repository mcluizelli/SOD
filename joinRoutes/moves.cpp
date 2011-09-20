#include "moves.h"

Moves::Moves()
{
}
Moves::Moves(double value,double relativeValue, QList<SimpleMove> moves){
    this->value = value;
    this->moves = moves;
    this->relativeValue = relativeValue;
    this->hasCompoundMoviment = false;
}

void Moves::setDemandRoute(QList<double>demand){
    this->demandRoute = demand;
}

QList<double> Moves::getDemandRoutes(){
    return this->demandRoute;
}

void Moves::setHasCompoundMoviment(bool value){
    this->hasCompoundMoviment = value;
}

bool Moves::getHasCompoundMoviment(){
    return this->hasCompoundMoviment;
}

void Moves::setCutPoints(QList<int>cutPoints){
    this->cutPoints = cutPoints;
}

QList<int> Moves::getCutPoints() const{
    return this->cutPoints;
}
QList<int> Moves::getJoinRoutes(){
    return this->joinRoutes;
}

void Moves::setValue(double value){
    this->value = value;
}

void Moves::setRelativeValue(double relativeValue){
    this->relativeValue = relativeValue;
}

void Moves::setRoutes(QList<int>indexRoutes){
    this->indexRoutes = indexRoutes;
}

QList<SimpleMove> Moves::getSimpleMoves(){
    return this->moves;
}

double Moves::getRelativeValue() const{
    return this->relativeValue;
}

int Moves::getNumJoint(){
    return this->numJoin;
}

QList<int> Moves::getIndexRoutes()const{
    return this->indexRoutes;
}

void Moves::setJoinPoints(QList<int>joinPoints){
    this->joinPoints = joinPoints;
}

void Moves::setJoinRoutes(QList<int> joinRoutes){
    this->joinRoutes = joinRoutes;
}

void Moves::setNumJoin(int numJoin){
    this->numJoin = numJoin;
}

double Moves::getValue() const{
    return this->value;
}
