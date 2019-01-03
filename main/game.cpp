#include "game.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include "enemy.h"
#include <QMediaPlayer>
#include <QScrollBar>
Game::Game(QWidget *parent){

    //Creating a scene
    scene = new QGraphicsScene();
    //Setting scene size
    scene->setSceneRect(0, 0, 5000, 700);

    setBackgroundBrush(QBrush(QImage(":/images/background.png").scaled(1280, 700)));
    setScene(scene);
    //Setting off horizontal scroll bar
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Setting off vertical scroll bar
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Setting window size to be fixed
    setFixedSize(1280, 700);
    horizontalScrollBar()->setValue(1);
    //Create rectangle item
    Enemy * enemy = new Enemy();

    //Creating player object
    player = new Player(enemy);
    //Adding Enemy object to the scene
    scene->addItem(enemy);

    //Add Player object to the scene
    scene->addItem(player);

    //Create a score
    score = new Score();
    scene->addItem(score);


    //Setting position of the player
    player->setPos(width()/2 - player->sceneBoundingRect().width()/2, height()/2 - player->sceneBoundingRect().height()/2);
    //Setting position of the enemy
    enemy->setPos(0, height()/2 - enemy->sceneBoundingRect().height()/2);

    QTimer * timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this->score,SLOT(increase()));
    timer->start(50);

    QTimer * timer2 = new QTimer();
    connect(timer2,SIGNAL(timeout()),this->player,SLOT(advance()));
    timer2->start(5);

    //Playing background music
    QMediaPlayer * music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/sounds/gravityGuySoundtrack.mp3"));
    music->play();


    show();
}
