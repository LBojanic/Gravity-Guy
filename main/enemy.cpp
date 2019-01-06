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
    setPixmap(QPixmap(":/images/player/run/" + QString::number(enemyCurrentImage) + QString::number(gravity()) + ".png").scaled(125, 125));

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
    setPixmap(QPixmap(":/images/player/run/" + QString::number(enemyCurrentImage) + QString::number(gravity()) + ".png").scaled(125, 125));
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
            < 3) {
            setGravity(!gravity());
            coordinatesWhereEnemyChanges.pop_front();
    } else {
        QTimer::singleShot(2, this, SLOT(spaceEvent()));
    }
}

QGraphicsPixmapItem* Enemy::collidesWithBlocks(QList<QGraphicsPixmapItem *> blocks) {
    //we go through our list of blocks and ask if enemy collides with some of them
    for(int i = 0; i < blocks.length(); i++) {
        if(collidesWithItem(blocks[i]))
            return blocks[i];
    }
    //otherwise we return null
    return nullptr;
}
