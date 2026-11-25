#include "goomba.h"
#include <QTimer>

Goomba::Goomba(double x, double y, const QString &imagePath)
    : Enemy(x, y, imagePath)
{
    patrolSpeed = 1.5;
    health = 20;
    maxHealth = 20;
    contactDamage = 20;

    // Like the original game: walk until hitting something, no fixed patrol area
    setPatrolRange(-1e9, 1e9);

    walkFrame1 = QPixmap(imagePath);
    walkFrame2 = QPixmap(":/images/goomba_walk2.png");
    flatFrame  = QPixmap(":/images/goomba_flat.png");
}

void Goomba::update()
{
    if (isDead())
        return;

    patrol();
    applyGravity();
    move();

    // Walking animation: switch picture every 10 frames
    animationCounter++;
    setPixmap((animationCounter / 10) % 2 == 0 ? walkFrame1 : walkFrame2);
}

void Goomba::stomp()
{
    takeDamage(health);
    velocityX = 0;
    velocityY = 0;

    // Show the squashed Goomba on the ground for a moment, then remove it
    setPixmap(flatFrame);
    setOffset(0, 16);                                // flat picture is half as tall
    QTimer::singleShot(400, this, [this]() { hide(); });
}

void Goomba::vanish()
{
    takeDamage(health);
    velocityX = 0;
    velocityY = 0;
    hide();
}
