#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include <QPixmap>
#include <QList>

class Player : public Character
{
private:
    double speed = 4.0;
    double jumpStrength = 12.0;

    static const int MAX_LIVES = 3;
    int lives = MAX_LIVES;
    double spawnX;
    double spawnY;

    int damageCooldown = 0;

    // ---- Animation ----
    QPixmap standFrame;
    QPixmap jumpFrame;
    QPixmap deadFrame;
    QList<QPixmap> walkFrames;
    bool facingRight = true;
    int animationCounter = 0;

    void updateAnimation();
    void showFrame(const QPixmap &frame);

public:
    Player(double x, double y, const QString &imagePath);

    void update() override;
    void moveLeft();
    void moveRight();
    void stopMoving();
    void jump();
    void setOnGround(bool grounded) override;

    int getLives() const;
    void loseLife();
    void resetLives();
    void respawn();
    void setSpawnPoint(double x, double y);

    void takeDamage(int damage) override;
    void showDeathPose();
};

#endif // PLAYER_H
