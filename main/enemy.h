#ifndef ENEMY_H
#define ENEMY_H
#include <QGraphicsRectItem>
#include <QObject>
class Enemy : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    Enemy();
    void setGravity(int value);
    int gravity();
    virtual ~Enemy();

public slots:
    void move();
    void spaceEvent();
private:
    int m_gravity;
};

#endif // ENEMY_H
