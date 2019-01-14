#include "coin.h"

#include <QTimer>


Coin::Coin(int x, int y) : m_x(x), m_y(y)
{
    for(int i = 0; i < 6; i++) {
        QPixmap * tmp = new QPixmap(":/images/coins/coin" + QString::number(i) + ".png");
        animationFrames.push_back(tmp);
    }

    setPixmap(*animationFrames.first());
    int width = boundingRect().width();
    setPos(m_x + 125/2 - width/2, m_y);

    QTimer* timer = new QTimer();

    //setting timer for coin rotation
    connect(timer, SIGNAL(timeout()), this, SLOT(changeImage()));

    timer->start(100);
}

Coin::~Coin()
{
    for(auto i : animationFrames)
        delete i;
}

void Coin::changeImage()
{
    m_currentImage = (m_currentImage + 1) % 6;

    //changing the coin image to the next one
    setPixmap(*animationFrames.at(m_currentImage));
    int width = boundingRect().width();
    setPos(m_x + 125/2 - width/2, m_y);
}
