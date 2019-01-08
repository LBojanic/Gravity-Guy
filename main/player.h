#ifndef PLAYER_H
#define PLAYER_H
#include <QGraphicsRectItem>
#include <QObject>
#include "enemy.h"
#include <QMediaPlayer>
#include <QGraphicsPixmapItem>
class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Player(Enemy * enemy);
    void setGravity(int value);
    int gravity();
    virtual ~Player();
    QGraphicsPixmapItem* collidesWithBlocks(QList<QGraphicsPixmapItem *> blocks);
    QGraphicsPixmapItem* crashesIntoBlock(QList<QGraphicsPixmapItem *> blocks);

public slots:
    void move();
    void changeImage();
    void advance();
    void goToMiddle();
private:
    void keyPressEvent(QKeyEvent * event);
    int m_gravity;
    Enemy * m_enemy;
    QMediaPlayer * jumpSound;
    int playerCurrentImage;
};


#endif // PLAYER_H
