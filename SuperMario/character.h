#ifndef CHARACTER_H
#define CHARACTER_H

#include "gameobject.h"

class Character : public GameObject
{
public:
    Character(double x, double y, const QString &imagePath);

    virtual void update() = 0;
};

#endif // CHARACTER_H