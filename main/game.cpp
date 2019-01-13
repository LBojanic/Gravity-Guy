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
#include <QtMath>

Game::Game(QWidget *parent){
    Q_UNUSED(parent);
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
    Q_UNUSED(mapName);
    int x = 0;
    int y = 0;
    //initial coordinates for blocks
    //opening map file
    QFile file(":/maps/level1.txt");
    //if we can't open our map we return
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(nullptr, "error", file.errorString());
        return ;
    }
    mapVector.clear();
    blocks.clear();
    //making a stream from our file
    QTextStream in(&file);
    while(!in.atEnd()) {
        //we draw map line by line
        QString line = in.readLine();
        //iterating through line and creating blocks if needed
        mapVector.push_back(line);

    }
    file.close();
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
    int soundButtonYPos = 0 + 20;
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
    drawPanel(game->horizontalScrollBar()->value(), 0, game->width(), game->height(), Qt::black, 0.90);

    //setting the game over text
    QGraphicsPixmapItem* gameOverText = new QGraphicsPixmapItem();
    gameOverText->setPixmap(QPixmap(":images/gameoverText.png").scaled(1125*0.8, 163*0.8));
    gameOverText->setPos(game->horizontalScrollBar()->value()+this->width()/2-gameOverText->boundingRect().width()/2, 220);
    scene->addItem(gameOverText);

    Score* scoreGameOver = new Score();
    scoreGameOver->setPos(game->horizontalScrollBar()->value()+width()/2-scoreGameOver->width()/2,height()/2+50);
    scoreGameOver->setHeight(100);
    scoreGameOver->setFontSize(70);
    scoreGameOver->setTextPos(scoreGameOver->x() + 30, scoreGameOver->y() + scoreGameOver->height()/2 - scoreGameOver->getTextHeight()/2);
    scoreGameOver->setScore(game->score->getScore());

    //setting the restart button
    restartButton = new Button(QString("reload"), 100, 100);
    restartButton->setPos(game->horizontalScrollBar()->value()+width()/2-250,height()/2+50);
    scene->addItem(restartButton);
    connect(restartButton, SIGNAL(clicked()), this, SLOT(restart()));

    //setting the return to main menu button
    returnToMenuButton = new Button(QString("quit"), 100, 100);
    returnToMenuButton->setPos(game->horizontalScrollBar()->value()+width()/2+150,height()/2+50);
    scene->addItem(returnToMenuButton);
    connect(returnToMenuButton, SIGNAL(clicked()), this, SLOT(close()));
}

void Game::displayPausePanel(){
    //we use drawPanel() function to draw a semi transparent panel which pops up when pause button is clicked
    panel = drawPanel(game->horizontalScrollBar()->value(), 0, game->width(), game->height(), Qt::black, 0.90);

    //setting the pause text
    QGraphicsPixmapItem* pauseText = new QGraphicsPixmapItem();
    pauseText->setPixmap(QPixmap(":images/pauseText.png").scaled(0.8*818, 0.8*164));
    pauseText->setPos(game->horizontalScrollBar()->value()+this->width()/2-pauseText->boundingRect().width()/2, 220);
    pauseText->setParentItem(panel);
    scene->addItem(pauseText);


    //setting the resume button
    resumeButton = new Button(QString("play"), 100, 100);
    resumeButton->setPos(game->horizontalScrollBar()->value()+width()/2-150,height()/2+50);
    scene->addItem(resumeButton);
    connect(resumeButton, SIGNAL(clicked()), this, SLOT(resumeGame()));

    //setting the return to main menu button
    returnToMenuButton = new Button(QString("quit"), 100, 100);
    returnToMenuButton->setPos(game->horizontalScrollBar()->value()+width()/2+50,height()/2+50);
    scene->addItem(returnToMenuButton);
    connect(returnToMenuButton, SIGNAL(clicked()), this, SLOT(close()));
}

qreal Game::distance(qreal x1, qreal y1, qreal x2, qreal y2)
{
    return qSqrt((x1 - x2) * (x1 - x2)
           + (y1 - y2) * (y1 - y2));
}

QGraphicsRectItem* Game::drawPanel(int x, int y, int width, int height, QColor color, double opacity){
   //drawing a panel on which we can put buttons and text, that appear for example on game over
   QGraphicsRectItem *panel = new QGraphicsRectItem(x, y, width, height);
   QBrush brush;
   brush.setStyle(Qt::SolidPattern);
   brush.setColor(color);
   panel->setBrush(brush);
   panel->setOpacity(opacity);
   scene->addItem(panel);

   //now we want to draw the actual panel where the buttons will be placed
   QGraphicsPixmapItem *panel2 = new QGraphicsPixmapItem();
   panel2->setPixmap(QPixmap(":/images/metalTexture.png").scaled(600, 400));
   panel2->setPos(x+this->width()/2-300, y+this->height()/2-200);
   panel2->setOpacity(1);

   scene->addItem(panel2);
   panel2->setParentItem(panel);

   return panel;
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

    gameStartedInd = 1; //when we go back to the main menu we need to reset this indicator

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


    //here we call draw map method where we will draw levels
    std::string name("level1.txt");
    currentFrame = 12;
    readMap(name);

    soundButton = new Button(QString("soundOn"), 50, 50);
    int soundButtonXPos = this->width() - 100;
    int soundButtonYPos = 0 + 20;
    soundButton->setPos(soundButtonXPos, soundButtonYPos);
    soundIconIndicator = 1; //currently there is sound and the button is soundOn
    connect(soundButton, SIGNAL(clicked()), this, SLOT(changeSoundIcon()));
    scene->addItem(soundButton);

    //set pause button
    pauseButton = new Button(QString("pause"), 50, 50);
    int pauseButtonXPos = this->width() - 170;
    int pauseButtonYPos = 0 + 20;
    pauseButton->setPos(pauseButtonXPos, pauseButtonYPos);
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(pause()));
    scene->addItem(pauseButton);

    //Setting position of the player
    player->setPos(width()/2 - player->boundingRect().width()/2, height()/2 - player->sceneBoundingRect().height()/2);
    //Setting position of the enemy
    enemy->setPos(100, height()/2 - enemy->boundingRect().height()/2);
    score->setPos(20, 20);
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
        backgroundMusic->setMuted(true);
        soundIconIndicator = 0;
        if(gameStartedInd)
            game->player->getJumpSound()->setMuted(true);
        soundButton->changeButtonIcon("soundOff");
    }
    //if the indicator is 0 then by clicking on the soundButton music needs to be played, and the button icon
    //has to be set on soundOff
    else{
        game->backgroundMusic->setMuted(false);
        soundIconIndicator = 1;
        if(gameStartedInd)
            game->player->getJumpSound()->setMuted(false);
        soundButton->changeButtonIcon("soundOn");
    }
}

void Game::pause(){
    //when the user clicks on pause button, we want all the timers to stop
    disconnect(timerScore,SIGNAL(timeout()),this->score,SLOT(increase()));
    disconnect(timerPlayerMove,SIGNAL(timeout()),this->player,SLOT(advance()));
    disconnect(timerForMap, SIGNAL(timeout()), this, SLOT(drawFrame()));
    disconnect(game->player->timerMove, SIGNAL(timeout()), this->player, SLOT(move()));
    disconnect(game->player->timerChangeImagePlayer, SIGNAL(timeout()), this->player, SLOT(changeImage()));
    disconnect(game->player->m_enemy->timerEnemyMove, SIGNAL(timeout()), this->player->m_enemy, SLOT(move()));
    disconnect(game->player->m_enemy->timerChangeImageEnemy, SIGNAL(timeout()), this->player->m_enemy, SLOT(changeImage()));
    pauseButton->setEnabled(false);
    soundButton->setEnabled(false);
    displayPausePanel();
}

void Game::restart()
{

    horizontalScrollBar()->setValue(1);
    backgroundMusic->play();
    QTimer::singleShot(1, this, SLOT(start()));
}


void Game::resumeGame()
{
    //after the user presses on resume game button the game must go on
    //from where it was paused, and the resume button need to vanish
    scene->removeItem(resumeButton);
    delete resumeButton;
    scene->removeItem(returnToMenuButton);
    delete returnToMenuButton;
    scene->removeItem(panel);
    delete panel;


    pauseButton->setEnabled(true);
    soundButton->setEnabled(true);

    connect(timerScore,SIGNAL(timeout()),this->score,SLOT(increase()));
    connect(timerPlayerMove,SIGNAL(timeout()),this->player,SLOT(advance()));
    connect(timerForMap, SIGNAL(timeout()), this, SLOT(drawFrame()));
    connect(game->player->timerMove, SIGNAL(timeout()), this->player, SLOT(move()));
    connect(game->player->timerChangeImagePlayer, SIGNAL(timeout()), this->player, SLOT(changeImage()));
    connect(game->player->m_enemy->timerEnemyMove, SIGNAL(timeout()), this->player->m_enemy, SLOT(move()));
    connect(game->player->m_enemy->timerChangeImageEnemy, SIGNAL(timeout()), this->player->m_enemy, SLOT(changeImage()));

}
