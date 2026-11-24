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

    int health = 100;
    int maxHealth = 100;

public:
    Character(double x, double y, const QString &imagePath);

    virtual void update() = 0;
    void applyGravity();
    void move();

    double getVelocityX() const;
    double getVelocityY() const;
    void setVelocityX(double velocity);
    void setVelocityY(double velocity);
    bool isOnGround() const;
    virtual void setOnGround(bool grounded);

    virtual void takeDamage(int damage);
    int getHealth() const;
    int getMaxHealth() const;
    bool isDead() const;
};

#endif