#include "player.h"
#include <QTransform>

Player::Player(double x, double y, const QString &imagePath)
    : Character(x, y, imagePath)
{
    spawnX = x;
    spawnY = y;

    // Load every animation picture once
    smallFrames.stand = QPixmap(imagePath);
    smallFrames.jump  = QPixmap(":/images/mario_jump.png");
    smallFrames.walk  = { QPixmap(":/images/mario_walk1.png"),
                          QPixmap(":/images/mario_walk2.png"),
                          QPixmap(":/images/mario_walk3.png") };

    bigFrames.stand = QPixmap(":/images/mario_big.png");
    bigFrames.jump  = QPixmap(":/images/mario_big_jump.png");
    bigFrames.walk  = { QPixmap(":/images/mario_big_walk1.png"),
                        QPixmap(":/images/mario_big_walk2.png"),
                        QPixmap(":/images/mario_big_walk3.png") };

    deadFrame = QPixmap(":/images/mario_dead.png");
}

void Player::update()
{
    if (damageCooldown > 0)
        damageCooldown--;

    applyGravity();
    move();
    updateAnimation();

    // Blink while invincible after being hurt
    if (damageCooldown > 0)
        setOpacity((damageCooldown / 4) % 2 == 0 ? 1.0 : 0.3);
    else
        setOpacity(1.0);
}

void Player::updateAnimation()
{
    // Which way is Mario facing?
    if (velocityX > 0)
        facingRight = true;
    else if (velocityX < 0)
        facingRight = false;

    // In the air -> jump picture
    if (!onGround) {
        showFrame(frames().jump);
        return;
    }

    // Moving on the ground -> cycle the walking pictures
    if (velocityX != 0) {
        animationCounter++;
        int index = (animationCounter / 6) % frames().walk.size();
        showFrame(frames().walk[index]);
    }
    // Standing still
    else {
        animationCounter = 0;
        showFrame(frames().stand);
    }
}

void Player::showFrame(const QPixmap &frame)
{
    if (facingRight)
        setPixmap(frame);
    else
        setPixmap(frame.transformed(QTransform().scale(-1, 1)));  // mirror
}

void Player::showDeathPose()
{
    setOpacity(1.0);
    setPixmap(deadFrame);
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
    if (onGround) {
        velocityY = -jumpStrength;
        onGround = false;
    }
}

void Player::setOnGround(bool grounded)
{
    onGround = grounded;

    if (grounded && velocityY > 0)
        velocityY = 0;
}

int Player::getLives() const
{
    return lives;
}

void Player::loseLife()
{
    if (lives > 0)
        lives--;
}

void Player::resetLives()
{
    lives = MAX_LIVES;
}

void Player::respawn()
{
    setPos(spawnX, spawnY);
    health = maxHealth;
    velocityX = 0;
    velocityY = 0;
    onGround = false;
    damageCooldown = 60;

    shape = PlayerShape::SMALL;     // a new life starts small
    facingRight = true;
    animationCounter = 0;
    setOpacity(1.0);
    showFrame(frames().stand);
}

void Player::setSpawnPoint(double x, double y)
{
    spawnX = x;
    spawnY = y;
}

void Player::takeDamage(int damage)
{
    if (damageCooldown > 0 || damage <= 0 || isDead())
        return;

    if (shape == PlayerShape::BIG) {
        shrink();                       // the mushroom protects him once
        return;
    }

    Character::takeDamage(damage);
    damageCooldown = 60;
}

void Player::addLife()
{
    lives++;
}

const Player::Frames &Player::frames() const
{
    return shape == PlayerShape::BIG ? bigFrames : smallFrames;
}

PlayerShape Player::getShape() const
{
    return shape;
}

bool Player::grow()
{
    if (shape == PlayerShape::BIG)
        return false;

    shape = PlayerShape::BIG;
    setY(y() - smallFrames.stand.height());    // grow upwards: feet stay on the ground
    showFrame(frames().stand);
    return true;
}

void Player::shrink()
{
    if (shape == PlayerShape::SMALL)
        return;

    shape = PlayerShape::SMALL;
    setY(y() + smallFrames.stand.height());    // shrink downwards: feet stay on the ground
    showFrame(frames().stand);
    damageCooldown = 120;                       // 2 seconds of blinking invincibility
}
