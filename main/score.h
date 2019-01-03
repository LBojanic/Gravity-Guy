#ifndef SCORE_H
#define SCORE_H
#include <QGraphicsTextItem>
#include <QObject>
class Score : public QGraphicsTextItem{
    Q_OBJECT
public:
    Score(QGraphicsItem * parent = 0);
    int score();

public slots:
    void increase();

private:
    int m_score;
};

#endif // SCORE_H
