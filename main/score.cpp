#include "score.h"
#include <QFont>

Score::Score(QGraphicsItem *parent) : QGraphicsTextItem(parent) {
    //initializing score to 0
    m_score = 0;
    //drawing the score text , QString converts number to string
    setPlainText("Score " + QString::number(m_score));
    //Setting default color for our score tet
    setDefaultTextColor(Qt::blue);
    //Passing font object QFont(nameOfFont, sizeOfFont);
    setFont(QFont("times", 16));


}

void Score::increase() {
    m_score ++ ;
    //Redrawing the score text , QString converts number to string
    setPlainText("Score " + QString::number(m_score));
}

int Score::score() {
    return m_score;
}
