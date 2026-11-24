#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"

class Enemy : public Character
{
public:
    Enemy(double x, double y, const QString &imagePath);

    virtual void update() = 0;
};

#endif