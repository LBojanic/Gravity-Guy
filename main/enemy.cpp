#include "enemy.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QtMath>
#include <QDebug>
#include "globals.h"
Enemy::Enemy()
{
    enemyCurrentImage = 0;
    //setting initial gravity
    setGravity(1);
    setPixmap(QPixmap(":/images/player/run/enemy" + QString::number(enemyCurrentImage) + QString::number(gravity()) + ".png").scaled(125, 125));

    //creating timer for movement
    timerEnemyMove = new QTimer(this);
    //setting its slots and signals
    connect(timerEnemyMove, SIGNAL(timeout()), this, SLOT(move()));
    //calling timer
    timerEnemyMove->start(5);




}
//Gravity setter
void Enemy::setGravity(int value)
{
    m_gravity = value;
}
//Gravity getter
int Enemy::gravity()
{
    return m_gravity;
}
//Enemy destructor
Enemy::~Enemy()
{

}

void Enemy::changeImage()
{
    //we go through images in cyclic manner and change them
    enemyCurrentImage = (enemyCurrentImage + 1) % 8;
    //setting new image
    setPixmap(QPixmap(":/images/player/run/enemy" + QString::number(enemyCurrentImage) + QString::number(gravity()) + ".png").scaled(125, 125));
}

void Enemy::goToPosition()
{
    setPos(x() + 1, y());
}

//Movement function
void Enemy::move()
{
    if(gravity() == 1)
    {

        if (game->player->y() >= game->height()) {
            //this is an animation when player goes beyond window's limits
            setPos(x(), y() + 2);
        } else if(y() + sceneBoundingRect().height() < scene()->height() && !collidesWithBlocks(game->blocks)) {
            //this is regular movement
            setPos(x(), y() + 1);
        }
        if(y() + sceneBoundingRect().height() >= scene()->height() + this->boundingRect().height() && !collidesWithBlocks(game->blocks)) {
            //if our enemy goes beneath window's lower limit then its gameover, otherwise translate
            game->gameOver();
        }
    }
    else
    {
        if(game->player->y() <= 0) {
            //this is an animation when player goes above window's limits
            setPos(x(), y() - 2);
        } else if(y() > 0 && !collidesWithBlocks(game->blocks)) {
            //this is regular movement
            setPos(x(), y() - 1);
        }
        if(y() <= -this->boundingRect().height() && !collidesWithBlocks(game->blocks)) {
            //if our enemy goes above window's upper limit then its gameover, otherwise translate
            game->gameOver();
        }

    }
}

//this is a slot for a signal that player emmits when space key is pressed
void Enemy::spaceEvent()
{
    setGravity(!gravity());
}

QGraphicsPixmapItem* Enemy::collidesWithBlocks(QList<QGraphicsPixmapItem *> blocks) {
    mutex->lock();
    qreal playerTop = pos().y(); //player top coordinate
    qreal playerBottom = pos().y() + boundingRect().height(); //player bottom coordinate
    qreal playerLeft = pos().x(); // player left coordinate
    qreal playerRight = pos().x() + boundingRect().width(); // player right coordinate
    for(int i = 0; i < blocks.length(); i++) {
        qreal blockTop = blocks[i]->pos().y(); //block top coordinate
        qreal blockBottom = blocks[i]->pos().y() + blocks[i]->boundingRect().height();//block bottom coordinate
        qreal blockLeft = blocks[i]->pos().x();// block left coordinate
        qreal blockRight = blocks[i]->pos().x() + blocks[i]->boundingRect().width();// block right coordinate

        //if gravity is 1 then player is moving downward so we need to check if he touches block top with his bottom meaning
        //the difference between player bottom and block top should be max 1 pixel since player moves 1 pixel down with gravity
        // after that we must check if players left edge or right edge is between block's x left and rigth edges
        // we have 2 cases, players right edge is between block's left and right and case where player's left edge is between
        if(gravity() == 1 && qFabs(playerBottom - blockTop) < 1 && ((playerLeft <= blockRight && playerLeft >= blockLeft) || (playerRight >= blockLeft && playerRight <= blockRight))) {
            mutex->unlock();
            return blocks[i];
        }
        //if gravity is 0 then player is moving upward so we need to check if he touches block bottom with his top
        //testing for x coordinates is the same as case above
        if(gravity() == 0 && qFabs(playerTop - blockBottom) < 1 && ((playerRight >= blockLeft && playerRight <= blockRight) || (playerLeft >= blockLeft && playerLeft <= blockRight))) {
            mutex->unlock();
            return blocks[i];
        }
    }
    mutex->unlock();
    return nullptr;
}

QGraphicsPixmapItem *Enemy::crashesIntoBlock(QList<QGraphicsPixmapItem *> blocks)
{
    qreal playerTop = pos().y(); //player top coordinate
    qreal playerBottom = pos().y() + boundingRect().height(); //player bottom coordinate
    qreal playerRight = pos().x() + boundingRect().width(); // player right coordinate
    mutex->lock();
    for(int i = 0; i < blocks.length(); i++) {
        qreal blockTop = blocks[i]->pos().y(); //block top coordinate
        qreal blockBottom = blocks[i]->pos().y() + blocks[i]->boundingRect().height();//block bottom coordinate
        qreal blockLeft = blocks[i]->pos().x();// block left coordinate

        if(qFabs(playerRight - blockLeft) <= 2 && ((playerTop <= blockBottom && playerTop >= blockTop) || (playerBottom >= blockTop && playerBottom <= blockBottom))) {
            mutex->unlock();
            return blocks[i];
        }
    }
    mutex->unlock();
    return nullptr;
}
