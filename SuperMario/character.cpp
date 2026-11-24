#include "character.h"

Character::Character(double x, double y, const QString &imagePath)
    : GameObject(x, y, imagePath)
{
}

void Character::applyGravity()
{
    if (!onGround)
    {
        velocityY += gravity;
    }
}

void Character::move()
{
    setPos(x() + velocityX, y() + velocityY);
}