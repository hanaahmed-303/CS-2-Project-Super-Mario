#ifndef FLAG_H
#define FLAG_H

#include "gameobject.h"
#include <QGraphicsPixmapItem>

// The flagpole at the end of the level: a tall pole on a block,
// with a flag at the top that slides down when Mario grabs the pole.
class Flag : public GameObject
{
public:
    Flag(double x, double y);
    void update() override {}
    bool activate();
    bool isActivated() const { return activated; }

    double getPoleX() const;            // x of the pole in the world
    double getPoleTop() const;          // top of the pole
    double getPoleBottom() const;       // top of the block at its base
    bool lowerCloth(double step);       // slide the flag down; true once it's at the bottom

private:
    bool activated = false;
    QGraphicsPixmapItem *cloth;         // the flag itself (a child, moves with the pole)

    static constexpr double POLE_OFFSET = 14;   // pole's x inside the picture
    static constexpr double BASE_HEIGHT = 32;   // the block at the bottom
};

#endif
