#include "enemy.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QKeyEvent>

Enemy::Enemy()
{
    setRect(0, 0, 100, 100);
    setGravity(1);
    //make rect focusable so we can use keyboard events
    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));

    timer->start(30);
}

void Enemy::setGravity(int value)
{
    m_gravity = value;
}

int Enemy::gravity()
{
    return m_gravity;
}

Enemy::~Enemy()
{

}

void Enemy::move()
{
    if(gravity() == 1)
    {
        if(y() + rect().height() < scene()->height()) {
            setPos(x(), y() + 10);
        } else {
            // gameover
        }
    }
    else
    {
        if(y() > 0) {
            setPos(x(), y() - 10);
        } else {
            // gameover
        }

    }
}

void Enemy::spaceEvent()
{
    setGravity(!gravity());
}
