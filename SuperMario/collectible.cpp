#include "collectible.h"

int Collectible::collect()
{
    if (collected)
        return 0;

    collected = true;
    hide();
    return points();
}

Coin::Coin(double x, double y)
    : Collectible(x, y, QStringLiteral(":/images/coin.png"))
{
    setOffset(6, 4);
}
