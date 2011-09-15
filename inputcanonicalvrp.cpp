#include "inputcanonicalvrp.h"
#include "sod.h"
#include <iostream>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QDebug>

using namespace std;

InputCanonicalVRP::InputCanonicalVRP()
{

}

InputCanonicalVRP::~InputCanonicalVRP(){

}

SOD InputCanonicalVRP::read(){

    SOD t;
    int numOrders = 0, numEquip = 0, indexOrder = 0;
    int xDepot = 0, yDepot = 0;
    double demand = 0.0, routeTime = 0, lixo = 0;
    long long int x  = 0, y = 0;
    cin>>numOrders;
    numEquip = 1;
    t.setNumberDepot(numEquip);
    cin>>routeTime;
    //t.setRouteTime(routeTime);
    cin>>lixo;
    cin>>lixo;
    cin>>xDepot;
    cin>>yDepot;

   for(int i = 0; i < numOrders; i++){
        cin>>x;
        cin>>y;
        cin>>demand;
        t.addOrder(indexOrder, x,y,demand,-1);
        indexOrder++;
    }

   t.addOrder(indexOrder, xDepot,yDepot,-1, -1);
   t.addDepot(0, indexOrder, xDepot, yDepot, 0, routeTime);
   t.setNumberDepot(1);

   return t;

}
SOD InputCanonicalVRP::read(QString inst){

    SOD t;
    bool ok;
    int numOrders = 0 , numEquip = 0 , indexOrder = 0;
    int xDepot, yDepot;
    double demand = 0.0, routeTime;
    long long int x  = 0, y = 0;

    QFile file(inst);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Erro ao abrir arquivo de leitura";
        return t;
    }

    QTextStream in(&file);

    //Primeira Linha
    QString line = in.readLine();
    QStringList lst = line.split(" ");
    numOrders = lst.at(0).toInt(&ok, 10);
    numEquip = 1;
    t.setNumberDepot(numEquip);
    routeTime = lst.at(1).toInt(&ok, 10);
    //t.setRouteTime(routeTime);
    lst.empty();

    //Segunda Linha
    line = in.readLine();
    lst = line.split(" ", QString::SkipEmptyParts);
    xDepot = lst.at(0).toInt(&ok, 10);
    yDepot = lst.at(1).toInt(&ok, 10);
    lst.empty();

    //Demais linhas do arquivo
    while(!in.atEnd()){

        line = in.readLine();
        lst = line.split(" ", QString::SkipEmptyParts);
        x = lst.at(0).toInt(&ok, 10);
        y = lst.at(1).toInt(&ok, 10);
        demand = lst.at(2).toInt(&ok, 10);
        t.addOrder(indexOrder, x,y,demand,-1);
        indexOrder++;
        lst.empty();
    }
    t.addOrder(indexOrder, xDepot,yDepot,-1, -1);
    t.addDepot(0, indexOrder, xDepot, yDepot, 0, routeTime);
    t.setNumberDepot(1);

    return t;

}
