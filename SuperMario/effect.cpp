#include "effect.h"
#include <QTimerEvent>

Effect::Effect(double x, double y, const QString &imagePath, int lifetime)
    : GameObject(x, y, imagePath), lifetime(lifetime)
{
    setZValue(20);              // in front of Mario and the blocks
    startTimer(16);             // its own little loop, ~60 times a second
}

void Effect::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    frames++;
    update();                   // each effect moves in its own way (polymorphism)

    if (frames >= lifetime)
        deleteLater();          // done: delete itself (also leaves the scene)
}

BrickDebris::BrickDebris(double x, double y, double velocityX, double velocityY)
    : Effect(x, y, QStringLiteral(":/images/brick.png"), 90),
      velocityX(velocityX), velocityY(velocityY)
{
    // A piece is a quarter of the brick picture
    setPixmap(pixmap().copy(0, 0, 16, 16));
    setTransformOriginPoint(8, 8);
}

void BrickDebris::update()
{
    velocityY += GRAVITY;
    moveBy(velocityX, velocityY);
    setRotation(rotation() + 12 * (velocityX > 0 ? 1 : -1));   // spins while flying
}
