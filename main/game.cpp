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
#include <QVector>
#include <limits>
#include "globals.h"
#include <button.h>
#include "player.h"

Game::Game(QWidget *parent){
    //in the Game constructor we set the scene, but to set the player, enemy, etc on the scene
    //we use start(), because we want those items to appear only after the user presses the 'play' button

    //Creating a scene
    scene = new QGraphicsScene();
    //Setting scene size
    scene->setSceneRect(0, 0, std::numeric_limits<double>::max(), 700);

    setBackgroundBrush(QBrush(QImage(":/images/background.png").scaled(1280, 700)));
    setScene(scene);
    //Setting off horizontal scroll bar
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Setting off vertical scroll bar
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Setting window size to be fixed
    setFixedSize(1280, 700);
    horizontalScrollBar()->setValue(1);
}

void Game::readMap(std::string & mapName)
{
    int x = 0;
    int y = 0;
    //initial coordinates for blocks
    //opening map file
    QFile file(":/maps/level1.txt");
    //if we can't open our map we return
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
        return ;
    }
    //making a stream from our file
    QTextStream in(&file);
    while(!in.atEnd()) {
        //we draw map line by line
        QString line = in.readLine();
        //iterating through line and creating blocks if needed
        mapVector.push_back(line);

    }

    for(int j = 0; j < currentFrame; j++)
    {
        for(int i = 0; i < mapVector.length(); i++)
        {
            QChar c = mapVector[i][j];

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
            y += 70;
        }
        x += 125;
        y = 0;
    }


    timerForMap = new QTimer();
    connect(timerForMap, SIGNAL(timeout()), this, SLOT(drawFrame()));
    timerForMap->start(400);
}

void Game::displayMainMenu(){
    //title
    QGraphicsTextItem* title = new QGraphicsTextItem(QString("Gravity guy"));
    //setting title font and size
    QFont titleFont("Helvetica", 50); //TODO: maybe change the font and font size
    title->setFont(titleFont);
    //position for the title
    int titleXPos = this->width()/2 - title->boundingRect().width()/2;
    int titleYPos = 150; //TODO: maybe change the position of the title
    title->setPos(titleXPos, titleYPos);
    scene->addItem(title);

    //creating the 'play' button
    Button* playButton = new Button(QString("Play"));
    int playButtonXPos = this->width()/2 - playButton->boundingRect().width()/2;
    int playButtonYPos = 350; //TODO: maybe change the play button position
    playButton->setPos(playButtonXPos, playButtonYPos);
    connect(playButton,SIGNAL(clicked()),this,SLOT(start()));
    scene->addItem(playButton);

    //creating the 'quit' button
    Button* quitButton = new Button(QString("Quit"));
    int quitButtonXPos = this->width()/2 - quitButton->boundingRect().width()/2;
    int quitButtonYPos = 450; //TODO: maybe change the quit button position
    quitButton->setPos(quitButtonXPos, quitButtonYPos);
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(quitButton);
}

void Game::drawFrame()
{
    mutex->lock();
    if(!blocks.isEmpty()) {
        auto blockToBeRemoved = blocks.first();
        blocks.removeFirst();
        scene->removeItem(blockToBeRemoved);
        delete blockToBeRemoved;
    }
    mutex->unlock();
    currentFrame++;
    int x = currentFrame * 125;
    int y = 0;
    for(auto row : mapVector) {
        if(row[currentFrame % mapVector[0].length()] == '#')
        {
            Block * a = new Block(x, y);
            mutex->lock();
            blocks.append(a);
            mutex->unlock();
            //set block coordinates
            a->setPos(x, y);
            //add item to scene
            scene->addItem(a);
        }
        y += 70;
    }

}

void Game::start(){
    //start() is called after the user presses the 'play' button in the main menu, which starts the game

    //clear the screen
    scene->clear();

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
    currentFrame = 12;
    readMap(name);



    //Setting position of the player
    player->setPos(width()/2 - player->boundingRect().width()/2, height()/2 - player->sceneBoundingRect().height()/2);
    //Setting position of the enemy
    enemy->setPos(100, height()/2 - enemy->boundingRect().height()/2);
    score->setPos(10, 10);
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
}
