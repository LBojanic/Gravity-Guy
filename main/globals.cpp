#include "globals.h"
Game * game = 0;
QList<QPair<qreal, qreal>> coordinatesWhereEnemyChanges;
QMutex * mutex = new QMutex();
