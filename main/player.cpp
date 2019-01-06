#include "player.h"
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QGraphicsScene>
#include "enemy.h"
#include <QDebug>
#include "game.h"
#include <QScrollBar>
#include "globals.h"
#include <QtMath>

Player::Player(Enemy * enemy) : m_enemy(enemy)
{
    //draw graphics
    playerCurrentImage = 0;
    //setting initial gravity for player
    setGravity(1);
    setPixmap(QPixmap(":/images/player/run/" + QString::number(playerCurrentImage) + QString::number(gravity()) + ".png").scaled(125, 125));

    //make rect focusable so we can use keyboard events
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    jumpSound = new QMediaPlayer();
    jumpSound->setMedia(QUrl("qrc:/sounds/jump.wav"));

    //creating timer object for movement
    QTimer * timer = new QTimer();
    //setting timer signals and slots
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    //starting the timer
    timer->start(5);

    //creating timer object for animation
    QTimer * timer2 = new QTimer();
    //setting timer signals and slots
    connect(timer2, SIGNAL(timeout()), this, SLOT(changeImage()));
    //starting the timer
    timer2->start(60);


}

//gravity setter
void Player::setGravity(int value)
{
    m_gravity = value;
}

//gravity getter
int Player::gravity()
{
    return m_gravity;
}
//player destructor
Player::~Player()
{

}
//player movement
void Player::move()
{
    //if gravity == 1 then he must be moving in downward position
    if(gravity() == 1)
    {
        //if our player goes beneath window lower limit then its gameover, otherwise translate
        if(y() + sceneBoundingRect().height() < scene()->height() && !collidesWithBlocks(game->blocks)) {
            setPos(x(), y() + 1);
        } else {
            // gameover
        }
    }
    else
    {
        //if our player goes beyond upper limit then its gameover, otherwise translate
        if(y() > 0 && !collidesWithBlocks(game->blocks)) {
            setPos(x(), y() - 1);
        } else {
            // gameover
        }

    }
}

void Player::changeImage()
{
    playerCurrentImage = (playerCurrentImage + 1) % 8;
    setPixmap(QPixmap(":/images/player/run/" + QString::number(playerCurrentImage) + QString::number(gravity()) + ".png").scaled(125, 125));
}

void Player::advance()
{
    m_enemy->setPos(m_enemy->x() + 2, m_enemy->y());
    setPos(x() + 2, y());
    game->horizontalScrollBar()->setValue(game->horizontalScrollBar()->value() + 2);
    if(game->horizontalScrollBar()->value() != game->horizontalScrollBar()->maximum())
        game->score->setPos(game->score->x() + 2, game->score->y());

}

//setting callback function for keypressevent
void Player::keyPressEvent(QKeyEvent *event)
{
    //if a space key is pressed, change gravity for player and after 500 ms send signal to enemy spaceEvent slot
    if(event->key() == Qt::Key_Space && collidesWithBlocks(game->blocks)) {
        setGravity(!gravity());
        //The following code plays a sound when a player jumps
        // The if-else is added because if we simply put 'jumpSound->play' it will only play once
        // so we need to check if a sound has already been played and repeating if it was.
        if(jumpSound->state() == QMediaPlayer::PlayingState) { //If a sound has been played set it to beginning
            jumpSound->setPosition(0);
        } else if (jumpSound->state() == QMediaPlayer::StoppedState) { // if a sound is stopped just play
            jumpSound->play();
        }
        coordinatesWhereEnemyChanges.push_back(QPair<qreal, qreal>(x(), y()));
        QTimer::singleShot(1, m_enemy, SLOT(spaceEvent()));
    }
}

QGraphicsPixmapItem* Player::collidesWithBlocks(QList<QGraphicsPixmapItem *> blocks) {

    for(int i = 0; i < blocks.length(); i++) {
        if(collidesWithItem(blocks[i]))
            return blocks[i];
    }
    return nullptr;
}

