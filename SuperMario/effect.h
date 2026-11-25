#ifndef EFFECT_H
#define EFFECT_H

#include "gameobject.h"

// A short visual effect that animates by itself and then deletes itself
// (nothing collides with it, and Game doesn't need to keep a list of them).
// Abstract: each effect decides how it moves in update().
class Effect : public GameObject
{
public:
    Effect(double x, double y, const QString &imagePath, int lifetime);

protected:
    void timerEvent(QTimerEvent *event) override;
    int frames = 0;             // how many frames it has been alive

private:
    int lifetime;               // frames before it removes itself
};

// One of the 4 pieces that fly out when Big Mario breaks a brick
class BrickDebris : public Effect
{
public:
    BrickDebris(double x, double y, double velocityX, double velocityY);
    void update() override;

private:
    double velocityX;
    double velocityY;
    static constexpr double GRAVITY = 0.5;
};

#endif // EFFECT_H
