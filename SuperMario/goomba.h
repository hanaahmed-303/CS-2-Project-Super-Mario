#ifndef GOOMBA_H
#define GOOMBA_H

#include "enemy.h"

class Goomba : public Enemy
{
public:
    Goomba(double x, double y, const QString &imagePath);

    void update() override;
    void stomp();
};

#endif