#include "powerup.h"
#include <algorithm>

PowerUp::PowerUp(double x, double y, const QString &imagePath)
    : Collectible(x, y, imagePath)
{
    setZValue(-0.5);                // behind the ? block while coming out of it
}

bool PowerUp::isSolid(Obstacle *obstacle)
{
    // Every obstacle is a wall for a power-up, spikes included
    // (spikes only hurt Mario; for a mushroom they are just blocks)
    return dynamic_cast<StaticObstacle *>(obstacle) != nullptr ||
           dynamic_cast<MovingObstacle *>(obstacle) != nullptr;
}

void PowerUp::move(const QList<Obstacle *> &obstacles, double levelBottom)
{
    if (isCollected() || !isVisible())
        return;

    // 1. Rise slowly out of the ? block
    if (emergeFrames > 0) {
        moveBy(0, -1);
        emergeFrames--;
        if (emergeFrames == 0)
            setZValue(0);           // fully out: draw it in front again
        return;
    }

    // 2. Slide sideways; turn around when hitting a wall.
    //    Only the middle of the mushroom is checked (2 px less at the top and bottom),
    //    so the floor it stands on is never mistaken for a wall.
    moveBy(velocityX, 0);
    for (Obstacle *obstacle : obstacles) {
        if (!isSolid(obstacle))
            continue;

        QRectF wall = obstacle->getBounds();
        QRectF body = getBounds().adjusted(0, 2, 0, -2);
        if (!body.intersects(wall))
            continue;

        if (velocityX > 0)
            moveBy(wall.left() - getBounds().right(), 0);   // stop right against the wall
        else
            moveBy(wall.right() - getBounds().left(), 0);
        velocityX = -velocityX;                              // and turn around
        break;
    }

    // 3. Fall with gravity; land on top of blocks.
    //    Only the inner width is checked (2 px less on each side),
    //    so a wall next to it is never mistaken for a floor.
    velocityY = std::min(velocityY + 0.5, 10.0);
    moveBy(0, velocityY);
    for (Obstacle *obstacle : obstacles) {
        if (!isSolid(obstacle))
            continue;

        QRectF block = obstacle->getBounds();
        QRectF feet = getBounds().adjusted(2, 0, -2, 0);
        if (!feet.intersects(block))
            continue;

        if (velocityY > 0)
            moveBy(0, block.top() - getBounds().bottom());   // land exactly on top
        else
            moveBy(0, block.bottom() - getBounds().top());   // bump from below
        velocityY = 0;
        break;
    }

    // 4. Fell into a pit -> gone
    if (getBounds().top() > levelBottom)
        hide();
}

Mushroom::Mushroom(double x, double y)
    : PowerUp(x, y, QStringLiteral(":/images/mushroom.png"))
{
}

void Mushroom::applyTo(Player *player)
{
    player->grow();
}
