#ifndef ENEMY_H
#define ENEMY_H
#include <QGraphicsRectItem>
#include <QObject>
class Enemy : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Enemy();
    void setGravity(int value);
    int gravity();
    virtual ~Enemy();

public slots:
    void move();
    void spaceEvent();
    void changeImage();
private:
    int m_gravity;
    int enemyCurrentImage;
};

#endif // ENEMY_H
