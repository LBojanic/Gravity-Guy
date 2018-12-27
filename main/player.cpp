#include "player.h"
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>

Player::Player()
{
    setRect(0, 0, 100, 100);
    setGravity(1);
    //make rect focusable so we can use keyboard events
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    QTimer * timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(50);
}

void Player::setGravity(int value)
{
    m_gravity = value;
}

int Player::gravity()
{
    return m_gravity;
}

Player::~Player()
{

}

void Player::move()
{
    if(gravity() == 1)
    {
        setPos(x(), y() + 10);
    }
    else
    {
        setPos(x(), y() - 10);

    }
}

void Player::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space) {
        setGravity(!gravity());
    }
}

