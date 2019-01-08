#ifndef GLOBALS_H
#define GLOBALS_H
#include "game.h"
#include <QMutex>
extern Game * game;
extern QMutex * mutex;
extern QList<QPair<qreal, qreal>> coordinatesWhereEnemyChanges;
#endif // GLOBALS_H
