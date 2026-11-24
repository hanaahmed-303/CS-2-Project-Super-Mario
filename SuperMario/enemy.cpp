#include "enemy.h"

Enemy::Enemy(double x, double y, const QString &imagePath)
    : Character(x, y, imagePath)
{
    patrolStart = x - 100;
    patrolEnd = x + 100;
}

void Enemy::patrol()
{
    if(x() <= patrolStart)
        direction = 1;

    else if(x() >= patrolEnd)
        direction = -1;

    velocityX = patrolSpeed * direction;
}

void Enemy::setPatrolRange(double start, double end)
{
    patrolStart = start;
    patrolEnd = end;
}