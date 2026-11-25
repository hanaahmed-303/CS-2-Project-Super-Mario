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
    int contactDamage = 20;

public:
    Enemy(double x, double y, const QString &imagePath);

    virtual void update() = 0;
    virtual void stomp() = 0;                   // Mario jumped on it (each enemy reacts differently)
    virtual void vanish();                      // removed quietly (fell in a pit, hit by a shell)
    virtual bool touchedFromSide(const Character *mario);   // true = handled, Mario is NOT hurt

    void patrol();
    void setPatrolRange(double start, double end);
    void reverseDirection();
    int getContactDamage() const;
};

#endif