#include "collisionmanager.h"
#include <cmath>

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

        MovingObstacle *platform =
            dynamic_cast<MovingObstacle *>(obstacle);

        if(solid && solid->isHazard())
            continue;

        if(!solid && !platform)
            continue;

        QRectF current = character->getBounds();
        QRectF block = obstacle->getBounds();

        bool horizontalOverlap =
            current.right() > block.left() &&
            current.left() < block.right();

        if(platform)
        {
            QPointF displacement = platform->getDisplacement();
            QRectF oldBlock = block.translated(-displacement);

            if(std::abs(previousBounds.bottom() -
                         oldBlock.top()) < 1.0 &&
                previousBounds.right() > oldBlock.left() &&
                previousBounds.left() < oldBlock.right() &&
                character->getVelocityY() >= 0)
            {
                character->moveBy(displacement.x(),
                                  displacement.y());

                current = character->getBounds();

                character->setY(character->y() +
                                block.top() - current.bottom());

                character->setOnGround(true);
                continue;
            }
        }

        if(horizontalOverlap &&
            std::abs(current.bottom() - block.top()) < 0.01 &&
            character->getVelocityY() >= 0)
        {
            character->setOnGround(true);
            continue;
        }

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

void CollisionManager::checkHazards(
    Player *player,
    const QList<Obstacle *> &obstacles)
{
    for(Obstacle *obstacle : obstacles)
    {
        StaticObstacle *hazard =
            dynamic_cast<StaticObstacle *>(obstacle);

        if(hazard && hazard->isHazard())
        {
            if(player->getBounds().intersects(
                    hazard->getBounds()))
            {
                player->takeDamage(25);
            }
        }
    }
}

bool CollisionManager::checkEnemyCollision(
    Player *player,
    Enemy *enemy,
    const QRectF &previousBounds)
{
    if(player->isDead() || enemy->isDead())
        return false;

    QRectF playerBounds = player->getBounds();
    QRectF enemyBounds = enemy->getBounds();

    if(!playerBounds.intersects(enemyBounds))
        return false;

    if(player->getVelocityY() > 0 &&
        previousBounds.bottom() <= enemyBounds.top())
    {
        enemy->stomp();

        player->setY(player->y() +
                     enemyBounds.top() - playerBounds.bottom());

        player->setOnGround(false);
        player->setVelocityY(-8);

        return true;
    }

    // Some enemies react to a side touch without hurting Mario
    // (e.g. a Koopa shell that is standing still gets kicked)
    if(enemy->touchedFromSide(player))
        return false;

    player->takeDamage(enemy->getContactDamage());

    return false;
}