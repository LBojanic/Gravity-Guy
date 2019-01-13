#include "block.h"
#include <globals.h>
Block::Block(int x, int y, QString photoPath, int width, int height)
    : m_x(x), m_y(y), m_photoPath(photoPath), m_width(width), m_height(height)
{
    QString fullPath = ":/tiles/" + photoPath + ".png";
    setPixmap(QPixmap(fullPath).scaled(125, 70));
    setPos(m_x, m_y);
}

