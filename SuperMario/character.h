#ifndef CHARACTER_H
#define CHARACTER_H

#include "gameobject.h"

class Character : public GameObject
{
protected:
    double velocityX = 0;
    double velocityY = 0;
    double gravity = 0.5;
    bool onGround = false;

public:
    Character(double x, double y, const QString &imagePath);

    virtual void update() = 0;
    void applyGravity();
    void move();
};

#endif // CHARACTER_H