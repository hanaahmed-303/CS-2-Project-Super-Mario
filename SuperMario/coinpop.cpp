#include "coinpop.h"
#include <QTimerEvent>

CoinPop::CoinPop(double x, double y)
    : Coin(x, y)
{
    setZValue(4);               // just behind the block it comes out of
    startTimer(16);
}

void CoinPop::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    frames++;

    moveBy(0, velocityY);       // up fast, then back down
    velocityY += 0.55;

    if (frames >= LIFETIME)
        deleteLater();
}
