#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "globals.h"

#include <QDesktopWidget>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    game = new Game();
    return a.exec();
}
