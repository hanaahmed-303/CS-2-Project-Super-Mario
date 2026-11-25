#ifndef POWERUP_H
#define POWERUP_H

#include "collectible.h"
#include "obstacle.h"
#include "player.h"
#include <QList>

// A collectible that comes out of a ? block, slides along the ground,
// and changes Mario when he touches it.
class PowerUp : public Collectible
{
public:
    PowerUp(double x, double y, const QString &imagePath);

    // Called every frame by Game: rise out of the block, then slide and fall
    void move(const QList<Obstacle *> &obstacles, double levelBottom);
    bool isEmerging() const { return emergeFrames > 0; }

    virtual void applyTo(Player *player) = 0;   // what it does to Mario

protected:
    double velocityX = 2.0;
    double velocityY = 0;
    int emergeFrames = 32;          // rises 1 px per frame for 32 frames (one tile)

private:
    static bool isSolid(Obstacle *obstacle);
};

// The Super Mushroom: makes Mario big
class Mushroom : public PowerUp
{
public:
    Mushroom(double x, double y);
    void applyTo(Player *player) override;

private:
    int points() const override { return 1000; }
};

#endif // POWERUP_H
