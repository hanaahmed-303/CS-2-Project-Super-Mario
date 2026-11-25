#include "obstacle.h"
#include "level.h"
#include <algorithm>
#include <QPixmap>
#include <QTimer>

Ground::Ground(double x, double y)
    : StaticObstacle(x, y, QStringLiteral(":/images/ground.png"))
{
}

Brick::Brick(double x, double y)
    : StaticObstacle(x, y, QStringLiteral(":/images/brick.png"))
{
}

// Picks the right picture: top rim or body, left or right half
static QString pipeImage(bool isTop, bool isLeft)
{
    if (isTop)
        return isLeft ? QStringLiteral(":/images/pipe_top_left.png")
                      : QStringLiteral(":/images/pipe_top_right.png");
    return isLeft ? QStringLiteral(":/images/pipe_body_left.png")
                  : QStringLiteral(":/images/pipe_body_right.png");
}

Pipe::Pipe(double x, double y, bool isTop, bool isLeft)
    : StaticObstacle(x, y, pipeImage(isTop, isLeft))
{
}

QuestionBlock::QuestionBlock(double x, double y)
    : StaticObstacle(x, y, QStringLiteral(":/images/question.png"))
{
}

bool QuestionBlock::hit()
{
    if (used)
        return false;

    used = true;
    setPixmap(QPixmap(QStringLiteral(":/images/used_block.png")));  // now an empty block

    // Small bounce: jump up 8 px, come back down 100 ms later
    moveBy(0, -8);
    QTimer::singleShot(100, this, [this]() { moveBy(0, 8); });
    return true;
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
    const double endX = startX + 3 * Level::TILE_SIZE;
    const double nextX = std::clamp(x() + speed, startX, endX);
    displacement = QPointF(nextX - x(), 0);
    setX(nextX);
    if (nextX == startX || nextX == endX)
        speed = -speed;
}
