#include "button.h"
#include <QBrush>
#include <QGraphicsTextItem>
#include "globals.h"

Button::Button(QString path, int width, int height, QGraphicsItem *parent):QGraphicsPixmapItem (parent), m_path(path), m_width(width), m_height(height){
    //setting picture for the button
    setPixmap(QPixmap(":/images/buttons/" + path + ".png").scaled(width, height));

    //we want out button to be able to accept hover events, on default this option is set on false
    setAcceptHoverEvents(true);
}

Button::~Button()
{
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event){
    //when the mouse is pressed on a button we need to activate a signal
    emit clicked();
}



void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    //when the mouse hovers on the button area, we want to change the button picture
    setPixmap(QPixmap(":/images/buttons/" + m_path + "Hover.png").scaled(m_width, m_height));
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    //when the mouse cursor leaves the button area, we want to change the button picture to what it was before
    setPixmap(QPixmap(":/images/buttons/" + m_path + ".png").scaled(m_width, m_height));
}

void Button::changeButtonIcon(QString path)
{
    m_path = path;
    setPixmap(QPixmap(":/images/buttons/" + path + ".png").scaled(m_width, m_height));
}



