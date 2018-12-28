#ifndef PLAYER_H
#define PLAYER_H
#include <QGraphicsRectItem>
#include <QObject>
#include "enemy.h"
class Player : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Player(Enemy * enemy);
    void setGravity(int value);
    int gravity();
    virtual ~Player();
public slots:
    void move();
private:
    void keyPressEvent(QKeyEvent * event);
    int m_gravity;
    Enemy * m_enemy;
};


#endif // PLAYER_H
