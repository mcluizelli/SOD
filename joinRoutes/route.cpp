#include "route.h"

Route::Route()
{
}

Route::Route(QList<int>orders, double value, double demand, int indexRoute){
    this->orders = orders;
    this->value = value;
    this->demand = demand;
    this->indexRoute = indexRoute;
}

void Route::setOrders(QList<int>orders){
    this->orders = orders;
}

void Route::setDemand(double demand){
    this->demand = demand;
}

void Route::setIndexRoute(int indexRoute){
    this->indexRoute = indexRoute;
}

void Route::setValue(double value){
    this->value = value;
}

double Route::getDemand() const{
    return this->demand;
}

int Route::getIndexRoute() const{
    return this->indexRoute;
}

QList<int> Route::getOrders()const{
    return this->orders;
}

double Route::getValue() const{
    return this->value;
}
