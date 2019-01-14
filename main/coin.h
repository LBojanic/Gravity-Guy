#ifndef COIN_H
#define COIN_H

#include <QGraphicsPixmapItem>

class Coin : public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Coin(int x, int y);
    ~Coin();

public slots:
    void changeImage();

private:
    int m_currentImage;
    int m_x;
    int m_y;
    QList<QPixmap *> animationFrames;
};

#endif // COIN_H
