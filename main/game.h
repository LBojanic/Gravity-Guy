#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include "player.h"
#include "score.h"
#include <QVector>
#include <QString>

class Game: public QGraphicsView{
    Q_OBJECT
public:
    Game(QWidget * parent = 0);
    void readMap(std::string & mapName);
    QGraphicsScene * scene;
    Player * player;
    Score * score;
    QList<QGraphicsPixmapItem *> blocks;
    int currentFrame;
    QVector<QString> mapVector;

    void displayMainMenu();

public slots:
    void drawFrame();
    //start() needs to be a public slot because when the 'play' button is clicked
    //start is used to handle the clicked() signal
    void start();
private:
    QTimer * timerForMap;
};
#endif // GAME_H
