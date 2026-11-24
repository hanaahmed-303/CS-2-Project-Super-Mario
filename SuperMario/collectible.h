#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include "gameobject.h"

class Collectible : public GameObject
{
public:
    using GameObject::GameObject;
    void update() override {}
    int collect();
    bool isCollected() const { return collected; }

private:
    virtual int points() const = 0;
    bool collected = false;
};

class Coin : public Collectible
{
public:
    Coin(double x, double y);

private:
    int points() const override { return 10; }
};

#endif
