#ifndef FLAG_H
#define FLAG_H

#include "gameobject.h"

class Flag : public GameObject
{
public:
    Flag(double x, double y);
    void update() override {}
    bool activate();
    bool isActivated() const { return activated; }

private:
    bool activated = false;
};

#endif
