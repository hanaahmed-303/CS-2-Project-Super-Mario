#include "collisionmanager.h"

void CollisionManager::resolveObstacles(
    Character *character,
    const QRectF &previousBounds,
    const QList<Obstacle *> &obstacles)
{
    character->setOnGround(false);

    for(Obstacle *obstacle : obstacles)
    {
        StaticObstacle *solid =
            dynamic_cast<StaticObstacle *>(obstacle);

        if(!solid || solid->isHazard())
            continue;

        QRectF current = character->getBounds();
        QRectF block = obstacle->getBounds();

        if(!current.intersects(block))
            continue;

        if(previousBounds.bottom() <= block.top())
        {
            character->setY(character->y()
                            + block.top() - current.bottom());

            character->setOnGround(true);
        }
        else if(previousBounds.top() >= block.bottom())
        {
            character->setY(character->y()
                            + block.bottom() - current.top());

            character->setVelocityY(0);
        }
        else if(previousBounds.right() <= block.left())
        {
            character->setX(character->x()
                            + block.left() - current.right());

            character->setVelocityX(0);
        }
        else if(previousBounds.left() >= block.right())
        {
            character->setX(character->x()
                            + block.right() - current.left());

            character->setVelocityX(0);
        }
    }
}