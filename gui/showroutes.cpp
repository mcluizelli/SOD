#include "showroutes.h"

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <iostream>

ShowRoutes::ShowRoutes(SOD *routes, float fatorAumento, QString nameWindow)
{
    QGraphicsScene *scene = new QGraphicsScene();
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, 1024,1000);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);
    setWindowTitle(nameWindow);

    //Add the points
    for(int i = 0; i < routes->getNumOrders(); i++){
        Order *point = routes->getOrder(i);
        scene->addEllipse(fatorAumento * point->getCoordX(), fatorAumento * point->getCoordY(), 5, 5);
        QGraphicsTextItem *text = scene->addText(QString::number( point->getIndex()) );
        text->setPos(fatorAumento * point->getCoordX(), fatorAumento * point->getCoordY());
    }

    //Add the edges

    for(int iCont = 0; iCont < routes->getNumberDepot(); iCont++){

        Depot *opDepotAux = routes->getDepot(iCont);
        Order *oDepot = routes->getOrder(opDepotAux->getIndexOfOrder());

        for(int iCont2 = 0; iCont2 < opDepotAux->getRoutes().size(); iCont2++){

            Route *oRouteAux = opDepotAux->getRoute(iCont2);

            scene->addLine(fatorAumento * qreal(oDepot->getCoordX()), fatorAumento * qreal(oDepot->getCoordY())
                           ,fatorAumento * qreal(routes->getOrder(oRouteAux->getRoute()->at(0))->getCoordX())
                           ,fatorAumento * qreal(routes->getOrder(oRouteAux->getRoute()->at(0))->getCoordY()));

            for(int iCont3 = 0; iCont3 < oRouteAux->getRoute()->size() - 1; iCont3++){
                Order *oOrderAux1 = routes->getOrder(oRouteAux->getRoute()->at(iCont3));
                Order *oOrderAux2 = routes->getOrder(oRouteAux->getRoute()->at(iCont3 + 1));
                scene->addLine(fatorAumento * qreal(oOrderAux1->getCoordX()), fatorAumento * qreal(oOrderAux1->getCoordY())
                               ,fatorAumento * qreal(oOrderAux2->getCoordX()), fatorAumento * qreal(oOrderAux2->getCoordY() ));
            }

            scene->addLine(fatorAumento * qreal(oDepot->getCoordX()), fatorAumento * qreal(oDepot->getCoordY())
                           ,fatorAumento * qreal(routes->getOrder(oRouteAux->getRoute()->at(oRouteAux->getRoute()->size()-1))->getCoordX())
                           ,fatorAumento * qreal(routes->getOrder(oRouteAux->getRoute()->at(oRouteAux->getRoute()->size()-1))->getCoordY()));

        }
    }
}

ShowRoutes::ShowRoutes(SOD *routes, int iNumberDepot, float fatorAumento, QString nameWindow)
{
    QGraphicsScene *scene = new QGraphicsScene();
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, 1024,1000);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);
    setWindowTitle(nameWindow);

    //Add the points
    Depot *oDeptoAux = routes->getDepot(iNumberDepot);
    QList<int> oListOrders = oDeptoAux->getOrdersAlocated();

   for(int i = 0; i < oListOrders.size() ; i++){
        Order *point = routes->getOrder(i);
        scene->addEllipse(fatorAumento * point->getCoordX(), fatorAumento * point->getCoordY(), 5, 5);
        QGraphicsTextItem *text = scene->addText(QString::number( point->getIndex()) );
        text->setPos(fatorAumento * point->getCoordX(), fatorAumento * point->getCoordY());
    }

    //Add the edges
    Order *oDepot = routes->getOrder(oDeptoAux->getIndexOfOrder());

    for(int iCont2 = 0; iCont2 < oDeptoAux->getRoutes().size(); iCont2++){

        Route *oRouteAux = oDeptoAux->getRoute(iCont2);

        scene->addLine(fatorAumento * qreal(oDepot->getCoordX()), fatorAumento * qreal(oDepot->getCoordY())
                       ,fatorAumento * qreal(routes->getOrder(oRouteAux->getRoute()->at(0))->getCoordX())
                       ,fatorAumento * qreal(routes->getOrder(oRouteAux->getRoute()->at(0))->getCoordY()));

        for(int iCont3 = 0; iCont3 < oRouteAux->getRoute()->size() - 1; iCont3++){
            Order *oOrderAux1 = routes->getOrder(oRouteAux->getRoute()->at(iCont3));
            Order *oOrderAux2 = routes->getOrder(oRouteAux->getRoute()->at(iCont3 + 1));
            scene->addLine(fatorAumento * qreal(oOrderAux1->getCoordX()), fatorAumento * qreal(oOrderAux1->getCoordY())
                           ,fatorAumento * qreal(oOrderAux2->getCoordX()), fatorAumento * qreal(oOrderAux2->getCoordY() ));
        }

        scene->addLine(fatorAumento * qreal(oDepot->getCoordX()), fatorAumento * qreal(oDepot->getCoordY())
                       ,fatorAumento * qreal(routes->getOrder(oRouteAux->getRoute()->at(oRouteAux->getRoute()->size()-1))->getCoordX())
                       ,fatorAumento * qreal(routes->getOrder(oRouteAux->getRoute()->at(oRouteAux->getRoute()->size()-1))->getCoordY()));
   }


}

