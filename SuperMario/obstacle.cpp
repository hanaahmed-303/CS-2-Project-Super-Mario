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
