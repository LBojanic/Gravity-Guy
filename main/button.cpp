#include "button.h"
#include <QBrush>
#include <QGraphicsTextItem>
#include "globals.h"

Button::Button(QString name, QGraphicsItem *parent):QGraphicsRectItem (parent){
    //drawing the rectangle for the button
    setRect(0, 0, 200, 50);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkCyan);
    setBrush(brush);

    //button is the parent of the message
    message = new QGraphicsTextItem(name, this);

    //setting the message to be in the centre of the parent
    int xPos = rect().width()/2 - message->boundingRect().width()/2;
    int yPos = rect().height()/2 - message->boundingRect().height()/2;
    message->setPos(xPos, yPos);

    //we want out button to be able to accept hover events, on default this option is set on false
    setAcceptHoverEvents(true);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event){
    //when the mouse is pressed on a button we need to activate a signal
    emit clicked();
}



void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    //when the mouse hovers on the button area, we want to change the buttons color
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::cyan); //TODO: maybe change the button color
    setBrush(brush);
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    //when the mouse cursor leaves the button area, we want the color of the button to
    //go back to what it was before
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkCyan); //TODO: maybe change the button color
    setBrush(brush);
}
