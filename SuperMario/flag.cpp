#include "flag.h"

Flag::Flag(double x, double y)
    : GameObject(x, y, QStringLiteral(":/images/flag.png"))
{
    setOffset(0, -32);
}

bool Flag::activate()
{
    if (activated)
        return false;

    activated = true;
    return true;
}
