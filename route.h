#ifndef ROUTE_H
#define ROUTE_H

#include <QList>

class Route
{

private:

    QList<int> listOrder;
public:

    Route();
    void addOrder(int iIndexOrder);
    QList<int>* getRoute();
    void showRoute();
};

#endif // ROUTE_H
