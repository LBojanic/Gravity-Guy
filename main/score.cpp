#include "score.h"
#include "globals.h"
#include <QFont>

Score::Score(QGraphicsItem *parent) : QGraphicsTextItem(parent) {
    //initializing score to 0
    m_score = 0;
    //drawing the score text , QString converts number to string
    setPlainText("Score " + QString::number(m_score));
    //Setting default color for our score tet
    setDefaultTextColor(Qt::white);
    //Passing font object QFont(nameOfFont, sizeOfFont);
    QFont font = QFont("sans", 28);
    font.setBold(true);
    setFont(font);

}

void Score::increase() {
    if(!game->player->crashesIntoBlock(game->blocks))
        m_score ++ ;
    //Redrawing the score text , QString converts number to string
    setPlainText("Score " + QString::number(m_score));
}

int Score::score() {
    return m_score;
}
