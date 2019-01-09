#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class Button: public QObject, public QGraphicsRectItem{
    Q_OBJECT
public:
    Button(QString name, QGraphicsItem* parent=NULL);

    //when the mouse cursor presses on the button the signal clicked() will be emitted
    //slots connected to it will handle the response
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

    //when the mouse cursor enters the button area and leaves it we want the button to change it's color
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

signals:
    void clicked();

private:
    QGraphicsTextItem* message;
};
#endif // BUTTON_H
