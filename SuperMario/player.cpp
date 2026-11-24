#include "player.h"

Player::Player(double x, double y, const QString &imagePath)
    : Character(x, y, imagePath)
{
    spawnX = x;
    spawnY = y;
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

int Player::getLives() const
{
    return lives;
}

void Player::loseLife()
{
    if (lives > 0)
    {
        lives--;
    }
}

void Player::respawn()
{
    setPos(spawnX, spawnY);
    health = maxHealth;
    velocityX = 0;
    velocityY = 0;
    onGround = false;
}

void Player::setSpawnPoint(double x, double y)
{
    spawnX = x;
    spawnY = y;
}