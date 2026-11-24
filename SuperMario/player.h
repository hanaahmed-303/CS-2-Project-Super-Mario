#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

class Player : public Character
{
private:
    double speed = 4.0;
    double jumpStrength = 12.0;

public:
    Player(double x, double y, const QString &imagePath);

    void update() override;
    void moveLeft();
    void moveRight();
    void stopMoving();
    void jump();
    void setOnGround(bool grounded);
};

#endif // PLAYER_H