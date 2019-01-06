#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include "player.h"
#include "score.h"

class Game: public QGraphicsView{
public:
    Game(QWidget * parent = 0);
    void drawMap(std::string & mapName);
    QGraphicsScene * scene;
    Player * player;
    Score * score;
    QList<QGraphicsPixmapItem *> blocks;
};
#endif // GAME_H
