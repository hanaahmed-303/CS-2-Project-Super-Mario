#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

class Player : public Character
{
public:
    Player(double x, double y, const QString &imagePath);

    void update() override;
};

#endif // PLAYER_H