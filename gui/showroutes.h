#ifndef SHOWROUTES_H
#define SHOWROUTES_H

#include <QtGui/QGraphicsView>
#include <sod.h>

class ShowRoutes : public QGraphicsView
{
public:
    ShowRoutes(SOD *vrp, float fatorAumento,QString nameWindow);
};

#endif // SHOWROUTES_H
