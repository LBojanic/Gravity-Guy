#ifndef BLOCK_H
#define BLOCK_H
#include <QGraphicsPixmapItem>
class Block : public QGraphicsPixmapItem {
public:
    Block(int x, int y, int width = 125, int height = 70);
private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;

};

#endif // BLOCK_H
