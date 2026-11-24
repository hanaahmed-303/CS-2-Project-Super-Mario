#include "goomba.h"

Goomba::Goomba(double x, double y, const QString &imagePath)
    : Enemy(x, y, imagePath)
{
    patrolSpeed = 1.5;
    health = 20;
    maxHealth = 20;
    contactDamage = 20;
}

void Goomba::update()
{
    if(isDead())
        return;

    patrol();
    applyGravity();
    move();
}

void Goomba::stomp()
{
    takeDamage(health);
    velocityX = 0;
    velocityY = 0;
    hide();
}