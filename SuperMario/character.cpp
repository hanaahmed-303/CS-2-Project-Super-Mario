#include "character.h"

Character::Character(double x, double y, const QString &imagePath)
    : GameObject(x, y, imagePath)
{
}

void Character::applyGravity()
{
    if(!onGround)
        velocityY += gravity;
}

void Character::move()
{
    setPos(x() + velocityX, y() + velocityY);
}

void Character::takeDamage(int damage)
{
    if(damage <= 0)
        return;

    health -= damage;

    if(health < 0)
        health = 0;
}

int Character::getHealth() const
{
    return health;
}

int Character::getMaxHealth() const
{
    return maxHealth;
}

bool Character::isDead() const
{
    return health == 0;
}

double Character::getVelocityX() const
{
    return velocityX;
}

double Character::getVelocityY() const
{
    return velocityY;
}

void Character::setVelocityX(double velocity)
{
    velocityX = velocity;
}

void Character::setVelocityY(double velocity)
{
    velocityY = velocity;
}

bool Character::isOnGround() const
{
    return onGround;
}

void Character::setOnGround(bool grounded)
{
    onGround = grounded;

    if(grounded && velocityY > 0)
        velocityY = 0;
}