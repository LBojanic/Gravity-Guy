#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "player.h"
#include "enemy.h"
#include "game.h"


static Game * game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    game = new Game();

    return a.exec();
}
