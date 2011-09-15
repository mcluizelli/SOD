#include "mainwindow.h"
#include "showroutes.h"
#include "inputcanonicalvrp.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDockWidget>
#include "math.h"

MainWindow::MainWindow()
{
/*
    SOD routes;
    SOD original;
    InputCanonicalVRP input;
    routes = input.read("vrpnc1.txt");
    GilletMiller gm(routes,1);
    original = routes;
    routes = gm.runM();
    //SweepGrasp sg(routes, 0.6, routes->getNumOrders() * log(routes->getNumOrders()));
    //routes = sg.run();
    
    ShowRoutes *show = new ShowRoutes(routes, 10, "Rota Original");
    //ShowRoutes *showConstrutivo = new ShowRoutes(original, 10);
    QDockWidget *dock = new QDockWidget(tr("View Route"));
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    QDockWidget *dock2 = new QDockWidget(tr("teste"));
    dock2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    //dock2->setWidget(showConstrutivo);
    dock->setWidget(show);
    //this->addDockWidget(Qt::TopDockWidgetArea, dock2);
    this->addDockWidget(Qt::BottomDockWidgetArea, dock);
    /*ShowRoutes *showConstrutivo = new ShowRoutes(original, 10);
    QPushButton *button = new QPushButton("teste");
    QPushButton *button2 = new QPushButton("teste2");
    QPushButton *button3 = new QPushButton("teste3");
    QHBoxLayout *horizontal = new QHBoxLayout;
    QVBoxLayout *vertical1 = new QVBoxLayout;
    QVBoxLayout *vertical2 = new QVBoxLayout;

    vertical1->addWidget(button);
    horizontal->addLayout(vertical1);

    dock->setWidget(showConstrutivo);

    horizontal->addWidget(dock);
    horizontal->addWidget(show);

    vertical2->addWidget(button2);
    vertical2->addWidget(button3);
    horizontal->addLayout(vertical2);

    setLayout(horizontal);

    */
    //Inicia processo de desalocacao de memoria.

    /*
    delete routes;
    */

}
