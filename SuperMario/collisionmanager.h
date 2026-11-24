#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "character.h"
#include "obstacle.h"
#include "player.h"
#include <QList>

class CollisionManager
{
public:
    static void resolveObstacles(
        Character *character,
        const QRectF &previousBounds,
        const QList<Obstacle *> &obstacles);

    static void checkHazards(
        Player *player,
        const QList<Obstacle *> &obstacles);
};

#endif