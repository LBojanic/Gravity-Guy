#include "game.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include <QThread>
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

    //Playing background music
    backgroundMusic = new QMediaPlayer();
    backgroundMusic->setMedia(QUrl("qrc:/sounds/gravityGuySoundtrack.mp3"));
    backgroundMusic->play();
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
    //setting logo on our main menu panel
    QGraphicsPixmapItem* logo = new QGraphicsPixmapItem();
    logo->setPixmap(QPixmap(":images/buttons/logo.png").scaled(610, 310));
    int logoX = this->width()/2 - logo->boundingRect().width()/2;
    int logoY = 100;
    logo->setPos(logoX, logoY);
    scene->addItem(logo);

    //setting pictures of robots for the main menu
    QGraphicsPixmapItem* yellowRobot = new QGraphicsPixmapItem();
    yellowRobot->setPixmap(QPixmap(":images/player/run/yellowRobot.png").scaled(600, 600));
    yellowRobot->setPos(20, 70);
    scene->addItem(yellowRobot);
    QGraphicsPixmapItem* redRobot = new QGraphicsPixmapItem();
    redRobot->setPixmap(QPixmap(":images/player/run/redRobot.png").scaled(600, 600));
    int redRobotX = this->width() - redRobot->boundingRect().width() - 20;
    redRobot->setPos(redRobotX, 70);
    scene->addItem(redRobot);

    //adding a sound button with the option to mute and unmute game sound
    //which is followed by change of the sound button icon
    soundButton = new Button(QString("soundOn"), 50, 50);
    int soundButtonXPos = this->width() - 100;
    int soundButtonYPos = 0 + 50;
    soundButton->setPos(soundButtonXPos, soundButtonYPos);
    soundIconIndicator = 1; //currently there is sound and the button is soundOn
    connect(soundButton, SIGNAL(clicked()), this, SLOT(changeSoundIcon()));
    scene->addItem(soundButton);


    //creating the 'play' button
    Button* playButton = new Button(QString("play"), 100, 100);
    int playButtonXPos = this->width()/2 - playButton->boundingRect().width()/2 - 100;
    int playButtonYPos = 450;
    playButton->setPos(playButtonXPos, playButtonYPos);
    connect(playButton,SIGNAL(clicked()),this,SLOT(start()));
    scene->addItem(playButton);

    //creating the 'quit' button
    Button* quitButton = new Button(QString("quit"), 100, 100);
    int quitButtonXPos = this->width()/2 - quitButton->boundingRect().width()/2 + 100;
    int quitButtonYPos = 450;
    quitButton->setPos(quitButtonXPos, quitButtonYPos);
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(quitButton);
}


void Game::gameOver(){
    //setting sound effect on game over
    backgroundMusic->stop();

    QMediaPlayer* gameOverSound = new QMediaPlayer();
    gameOverSound->setMedia(QUrl("qrc:/sounds/GameOver.wav"));
    gameOverSound->play();


    //when gameOver() is called we need all the timers to stop
    disconnect(timerScore,SIGNAL(timeout()),this->score,SLOT(increase()));
    disconnect(timerPlayerMove,SIGNAL(timeout()),this->player,SLOT(advance()));
    disconnect(timerForMap, SIGNAL(timeout()), this, SLOT(drawFrame()));
    disconnect(game->player->timerMove, SIGNAL(timeout()), this->player, SLOT(move()));
    disconnect(game->player->timerChangeImagePlayer, SIGNAL(timeout()), this->player, SLOT(changeImage()));
    disconnect(game->player->m_enemy->timerEnemyMove, SIGNAL(timeout()), this->player->m_enemy, SLOT(move()));
    disconnect(game->player->m_enemy->timerChangeImageEnemy, SIGNAL(timeout()), this->player->m_enemy, SLOT(changeImage()));

    //we use drawPanel() function to draw a semi transparent panel which pops up when game over is called
    drawPanel(game->horizontalScrollBar()->value(), 0, game->width(), game->height(), Qt::black, 0.65);

    //now we want to draw the actual panel where the buttons will be placed
    drawPanel(game->horizontalScrollBar()->value()+width()/2-200,height()/2-200, 400, 400, Qt::lightGray, 0.75);

    //TODO: make buttons for game restart and quit
}

void Game::drawPanel(int x, int y, int width, int height, QColor color, double opacity){
   //drawing a panel on which we can put buttons and text, that appear for example on game over
   QGraphicsRectItem *panel = new QGraphicsRectItem(x, y, width, height);
   QBrush brush;
   brush.setStyle(Qt::SolidPattern);
   brush.setColor(color);
   panel->setBrush(brush);
   panel->setOpacity(opacity);
   scene->addItem(panel);
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
    timerScore = new QTimer();
    connect(timerScore,SIGNAL(timeout()),this->score,SLOT(increase()));
    timerScore->start(50);

    //creating timer for moving our view forward
    //we are not moving view actually, we move every object and simulate moving view
    timerPlayerMove = new QTimer();
    connect(timerPlayerMove,SIGNAL(timeout()),this->player,SLOT(advance()));
    timerPlayerMove->start(5);
}

void Game::changeSoundIcon()
{
    //if the indicator is 1 that means that there is sound currently, so the icon needs to change from soundOn
    //to soundOff and the music needs to be stopped
    if(soundIconIndicator == 1){
        backgroundMusic->stop();
        soundIconIndicator = 0;
        soundButton->changeButtonIcon("soundOff");
    }
    //if the indicator is 0 then by clicking on the soundButton music needs to be played, and the button icon
    //has to be set on soundOff
    else{
        game->backgroundMusic->play();
        soundIconIndicator = 1;
        soundButton->changeButtonIcon("soundOn");
    }
}
