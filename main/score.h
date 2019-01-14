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
    ~Score();
    int score();
    QGraphicsTextItem * text;
    QGraphicsPixmapItem * background;
    void setPos(int x, int y);
    int x();
    int y();
    int width();
    int height();
    void setHeight(int height);
    void setWidth(int width);
    void setScore(int score);
    void setFontSize(int size);
    int getScore();
    int getTextWidth();
    int getTextHeight();
    void setTextPos(int x, int y);

public slots:
    void increase();

private:
    int m_score;
    int m_x;
    int m_y;
    int m_height;
    int m_width;
    int m_font_size;
};

#endif // SCORE_H
