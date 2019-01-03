#include "enemy.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QKeyEvent>

Enemy::Enemy()
{
    //representing enemy as rectangle
    setRect(0, 0, 100, 100);
    //setting initial gravity
    setGravity(1);
    //creating timer for movement
    QTimer * timer = new QTimer(this);
    //setting its slots and signals
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    //calling timer
    timer->start(30);
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
//Movement function
void Enemy::move()
{
    //if gravity == 1 then he must be moving in downward position
    if(gravity() == 1)
    {
        //if his lower edge passes after our window's bottom limit then its gameover, otherwise translate a bit more
        if(y() + rect().height() < scene()->height()) {
            setPos(x(), y() + 10);
        } else {
            // gameover
        }
    }
    else
    {
        //if his upper edge passes after our window's upper limit then its gameover, otherwise translate a bit more
        if(y() > 0) {
            setPos(x(), y() - 10);
        } else {
            // gameover
        }

    }
}

//this is a slot for a signal that player emmits when space key is pressed
void Enemy::spaceEvent()
{
    setGravity(!gravity());
}
