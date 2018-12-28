#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "player.h"
#include "enemy.h"


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
    Enemy * enemy = new Enemy();

    Player * player = new Player(enemy);
    scene->addItem(enemy);

    //Add item to the scene
    scene->addItem(player);


    //Add a view and setting which scene we want to visualize
    QGraphicsView * view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->show();
    view->setFixedSize(800, 600);
    scene->setSceneRect(0, 0, 800, 600);

    player->setPos(view->width()/2 - player->rect().width()/2, view->height()/2 - player->rect().height()/2);
    enemy->setPos(0, view->height()/2 - enemy->rect().height()/2);

    return a.exec();
}
