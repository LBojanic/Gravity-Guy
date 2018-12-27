#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "player.h"


/*
    QGraphicsScene - kontejner za sve game objekte - mapa - samo informacije vezane za scenu
    QGraphicsItem - svaki objekat koji hocu da stavim na scenu je izveden iz ove klase - samo ova klasa i potklase idu na scenu
        - QGraphicsRectItem - izveden iz ovog gore i to je pravougaonik
    QGraphicsView - sluzi za prikazivanje scene - printovanje scene sa svim informacijama
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Creating a scene
    QGraphicsScene * scene = new QGraphicsScene();

    //Create rectangle item
    Player * rect = new Player();

    //Add item to the scene
    scene->addItem(rect);

    //Add a view and setting which scene we want to visualize
    QGraphicsView * view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->show();

    return a.exec();
}
