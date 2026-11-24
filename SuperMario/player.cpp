#include "player.h"

Player::Player(double x, double y, const QString &imagePath)
    : Character(x, y, imagePath)
{
}

void Player::update()
{
    applyGravity();
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

void Player::jump()
{
    if (onGround)
    {
        velocityY = -jumpStrength;
        onGround = false;
    }
}

void Player::setOnGround(bool grounded)
{
    onGround = grounded;

    if (grounded && velocityY > 0)
    {
        velocityY = 0;
    }
}