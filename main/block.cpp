#include "block.h"
Block::Block(int x, int y, int width, int height)
    : m_x(x), m_y(y), m_width(width), m_height(height)
{
    setPixmap(QPixmap(":/tiles/tile.png").scaled(125, 60));
    setPos(m_x, m_y);
}
