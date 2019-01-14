#include "score.h"
#include "globals.h"
#include <QFont>
#include <QFontDatabase>

Score::Score(QGraphicsItem *parent){
    Q_UNUSED(parent);
    //initializing score to 0
    m_score = 0;
    m_x = 0;
    m_y = 0;
    m_font_size = 45;

    text = new QGraphicsTextItem();
    //drawing the score text , QString converts number to stringz
    text->setPlainText(QString::number(m_score));
    //Setting default color for our score tet
    text->setDefaultTextColor(Qt::black);
    //Passing font object QFont(nameOfFont, sizeOfFont);
    QFontDatabase::addApplicationFont(":/fonts/digital-7.ttf");
    QFont font = QFont("digital-7", m_font_size);
    font.setBold(true);
    text->setFont(font);

    text->setPos(m_x + 20, m_y + 10);
    m_width = 200;
    m_height = 70;
    background = new QGraphicsPixmapItem();
    background->setPixmap(QPixmap(":/scoreBar/scoreBar.png").scaled(m_width, m_height));
    background->setPos(m_x, m_y);

    game->scene->addItem(background);
    game->scene->addItem(text);
}

Score::~Score()
{
    delete text;
    delete background;
}

void Score::increase() {
    if(!game->player->crashesIntoBlock(game->blocks))
        m_score ++ ;
    //Redrawing the score text , QString converts number to string
    text->setPlainText(QString::number(m_score));
}

int Score::score() {
    return m_score;
}

void Score::setPos(int x, int y)
{
    m_x = x;
    m_y = y;
    background->setPos(x, y);
    text->setPos(x + 20, y + 10);
}

int Score::x()
{
    return m_x;
}

int Score::y()
{
    return m_y;
}

int Score::width()
{
    return background->pixmap().width();
}

int Score::height()
{
    return background->pixmap().height();
}

void Score::setWidth(int width)
{
    m_width = width;
    background->setPixmap(QPixmap(":/tiles/tile.png").scaled(m_width, m_height));
}

void Score::setScore(int score)
{
    m_score = score;
    text->setPlainText(QString::number(m_score));
}

void Score::setFontSize(int size)
{
    m_font_size = size;
    text->setPlainText(QString::number(m_score));
}

int Score::getScore()
{
    return m_score;
}

int Score::getTextWidth()
{
    return text->boundingRect().width();
}

int Score::getTextHeight()
{
    return text->boundingRect().height();
}

void Score::setTextPos(int x, int y)
{
    text->setPos(x, y);
}

void Score::setHeight(int height)
{
    m_height = height;
    background->setPixmap(QPixmap(":/tiles/tile.png").scaled(m_width, m_height));
}
