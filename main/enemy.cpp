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
    QTimer * timer = new QTimer(this);
    //setting its slots and signals
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    //calling timer
    timer->start(5);


    //creating timer object for animation
    QTimer * timer2 = new QTimer();
    //setting timer signals and slots
    connect(timer2, SIGNAL(timeout()), this, SLOT(changeImage()));
    //starting the timer
    timer2->start(60);


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
    //if gravity == 1 then he must be moving in downward position
    if(gravity() == 1)
    {
        //if his lower edge passes after our window's bottom limit then its gameover, otherwise translate a bit more
        if(y() + sceneBoundingRect().height() < scene()->height() && !collidesWithBlocks(game->blocks)) {
            setPos(x(), y() + 1);
        } else {
            // gameover
        }
    }
    else
    {
        //if his upper edge passes after our window's upper limit then its gameover, otherwise translate a bit more
        if(y() > 0 && !collidesWithBlocks(game->blocks)) {
            setPos(x(), y() - 1);
        } else {
            // gameover
        }

    }
}

//this is a slot for a signal that player emmits when space key is pressed
void Enemy::spaceEvent()
{
    //if distance from enemy to location where player changed gravity gets smaller than 3
    // then we change enemy gravity, otherwise we recursively call slot
    if(qSqrt((coordinatesWhereEnemyChanges.first().first - x()) * (coordinatesWhereEnemyChanges.first().first - x())
             + (coordinatesWhereEnemyChanges.first().second - y()) * (coordinatesWhereEnemyChanges.first().second - y()))
            < 2) {
            setGravity(!gravity());
            coordinatesWhereEnemyChanges.pop_front();
    } else {
        QTimer::singleShot(2, this, SLOT(spaceEvent()));
    }
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
