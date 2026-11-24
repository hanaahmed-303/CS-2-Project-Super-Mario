#include "obstacle.h"
#include <algorithm>

Ground::Ground(double x, double y)
    : StaticObstacle(x, y, QStringLiteral(":/images/ground.png"))
{
}

Brick::Brick(double x, double y)
    : StaticObstacle(x, y, QStringLiteral(":/images/brick.png"))
{
}

Pipe::Pipe(double x, double y)
    : StaticObstacle(x, y, QStringLiteral(":/images/pipe.png"))
{
}

Spike::Spike(double x, double y)
    : StaticObstacle(x, y, QStringLiteral(":/images/spike.png"))
{
}

MovingPlatform::MovingPlatform(double x, double y)
    : MovingObstacle(x, y, QStringLiteral(":/images/platform.png")),
      startX(x)
{
}

void MovingPlatform::update()
{
    const double endX = startX + 96;
    const double nextX = std::clamp(x() + speed, startX, endX);
    displacement = QPointF(nextX - x(), 0);
    setX(nextX);
    if (nextX == startX || nextX == endX)
        speed = -speed;
}
