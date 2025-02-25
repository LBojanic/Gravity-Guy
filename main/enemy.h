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
    ~Enemy();
    QGraphicsPixmapItem* collidesWithBlocks(QList<QGraphicsPixmapItem *> blocks);
    QGraphicsPixmapItem* crashesIntoBlock(QList<QGraphicsPixmapItem *> blocks);
    QTimer * timerEnemyMove;

    QVector<QPixmap *> enemyPictures;

public slots:
    void move();
    void spaceEvent();
    void changeImage();
    void goToPosition();
private:
    int m_gravity;
    int enemyCurrentImage;
};

#endif // ENEMY_H
