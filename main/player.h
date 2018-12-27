#ifndef PLAYER_H
#define PLAYER_H
#include <QGraphicsRectItem>
#include <QObject>
class Player : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Player();
    void setGravity(int value);
    int gravity();
    virtual ~Player();
public slots:
    void move();
private:
    void keyPressEvent(QKeyEvent * event);
    int m_gravity;
};


#endif // PLAYER_H
