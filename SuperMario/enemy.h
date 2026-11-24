#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"

class Enemy : public Character
{
protected:
    double patrolSpeed = 2.0;
    double patrolStart;
    double patrolEnd;
    int direction = -1;

public:
    Enemy(double x, double y, const QString &imagePath);

    virtual void update() = 0;
    void patrol();
    void setPatrolRange(double start, double end);
};

#endif