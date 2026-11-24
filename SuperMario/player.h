#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

class Player : public Character
{
private:
    double speed = 4.0;
    double jumpStrength = 12.0;

    int lives = 3;
    double spawnX;
    double spawnY;

public:
    Player(double x, double y, const QString &imagePath);

    void update() override;
    void moveLeft();
    void moveRight();
    void stopMoving();
    void jump();
    void setOnGround(bool grounded);

    int getLives() const;
    void loseLife();
    void respawn();
    void setSpawnPoint(double x, double y);
};

#endif // PLAYER_H