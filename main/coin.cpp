#include "coin.h"

#include <QTimer>


Coin::Coin(int x, int y) : m_x(x), m_y(y)
{
    m_currentImage = 0;
    setPixmap(QPixmap(":/images/coins/coin" + QString::number(m_currentImage) + ".png"));
    int width = boundingRect().width();
    setPos(m_x + 125/2 - width/2, m_y);

    QTimer* timer = new QTimer();

    //setting timer for coin rotation
    connect(timer, SIGNAL(timeout()), this, SLOT(changeImage()));

    timer->start(100);
}

void Coin::changeImage()
{
    m_currentImage = (m_currentImage + 1) % 6;

    //changing the coin image to the next one
    setPixmap(QPixmap(":/images/coins/coin" + QString::number(m_currentImage) + ".png"));
    int width = boundingRect().width();
    setPos(m_x + 125/2 - width/2, m_y);
}
