#include "route.h"
#include <iostream>
#include <QList>

Route::Route()
{

}

void Route::showRoute(){

    foreach( int iOrder, this->listOrder){
        std::cout << iOrder << ", ";
    }
    std::cout << " " << std::endl;

}

void Route::addOrder(int iIndexOrder){
    this->listOrder.append(iIndexOrder);
}

QList<int>* Route::getRoute(){
    return &this->listOrder;
}
