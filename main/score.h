#ifndef SCORE_H
#define SCORE_H
#include <QGraphicsTextItem>
#include <QObject>
#include <QPainter>
#include <QPixmap>
class Score : public QObject{
    Q_OBJECT
public:
    Score(QGraphicsItem * parent = nullptr);
    int score();
    QGraphicsTextItem * text;
    QGraphicsPixmapItem * background;
    void setPos(int x, int y);
    int x();
    int y();
public slots:
    void increase();

private:
    int m_score;
    int m_x;
    int m_y;
};

#endif // SCORE_H
