#include "flag.h"
#include <QPixmap>
#include <algorithm>

Flag::Flag(double x, double y)
    : GameObject(x, y, QStringLiteral(":/images/flagpole.png"))
{
    // The picture is 11 tiles tall: its base block sits on the F tile of the map
    setOffset(0, BASE_HEIGHT - pixmap().height());

    // The flag hangs on the left of the pole, just under the ball on top
    cloth = new QGraphicsPixmapItem(QPixmap(QStringLiteral(":/images/flag_cloth.png")), this);
    cloth->setPos(POLE_OFFSET - cloth->pixmap().width(), offset().y() + 16);
}

bool Flag::activate()
{
    if (activated)
        return false;

    activated = true;
    return true;
}

double Flag::getPoleX() const
{
    return scenePos().x() + POLE_OFFSET;
}

double Flag::getPoleTop() const
{
    return sceneBoundingRect().top();
}

double Flag::getPoleBottom() const
{
    return sceneBoundingRect().bottom() - BASE_HEIGHT;
}

bool Flag::lowerCloth(double step)
{
    // Lowest position: the flag's bottom touches the top of the base block
    const double lowest = -cloth->pixmap().height();
    cloth->setY(std::min(cloth->y() + step, lowest));
    return cloth->y() >= lowest;
}
