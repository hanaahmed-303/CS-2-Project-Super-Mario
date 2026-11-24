#include "player.h"

Player::Player(double x, double y, const QString &imagePath)
    : Character(x, y, imagePath)
{
}

void Player::update()
{
    move();
}

void Player::moveLeft()
{
    velocityX = -speed;
}

void Player::moveRight()
{
    velocityX = speed;
}

void Player::stopMoving()
{
    velocityX = 0;
}