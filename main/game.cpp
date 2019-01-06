#include "game.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include "enemy.h"
#include "block.h"
#include <QMediaPlayer>
#include <QScrollBar>
#include <QFile>
#include <QDebug>
#include <QChar>
#include <QMessageBox>
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

    //here we call draw map method where we will draw levels
    std::string name("level1.txt");
    drawMap(name);

    //Setting position of the player
    player->setPos(width()/2 - player->sceneBoundingRect().width()/2, height()/2 - player->sceneBoundingRect().height()/2);
    //Setting position of the enemy
    enemy->setPos(0, height()/2 - enemy->sceneBoundingRect().height()/2);

    //timer for score increasing
    QTimer * timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this->score,SLOT(increase()));
    timer->start(50);

    //creating timer for moving our view forward
    //we are not moving view actually, we move every object and simulate moving view
    QTimer * timer2 = new QTimer();
    connect(timer2,SIGNAL(timeout()),this->player,SLOT(advance()));
    timer2->start(5);

    //Playing background music
    QMediaPlayer * music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/sounds/gravityGuySoundtrack.mp3"));
    music->play();


    show();
}

void Game::drawMap(std::string & mapName)
{
    int x = 0;
    int y = 0;
    //initial coordinates for blocks
    std::string line;
    //opening map file
    QFile file(":/maps/level1.txt");
    //if we can't open our map we return
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
        return ;
    }
    //making a stream from our file
    QTextStream in(&file);
    int i  = 0;
    while(!in.atEnd()) {
        //we draw map line by line
        QString line = in.readLine();
        //iterating through line and creating blocks if needed
        for(QChar c : line)
        {
            if(c == '#')
            {
                //make new block and add it to blocks list
                Block * a = new Block(x, y);
                blocks.append(a);
                //set block coordinates
                a->setPos(x, y);
                //add item to scene
                scene->addItem(a);
            }
            //increase x since we are iterating through line
            x += 125;
        }

        //increase y since we change line and set x to the beginning of the line(0)
        x = 0;
        y += 70;
    }

}
