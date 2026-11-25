#ifndef COINPOP_H
#define COINPOP_H

#include "collectible.h"

// The coin that jumps out of a ? block. It IS a Coin, but Mario never
// touches it: it is counted right away, flies up, falls back and disappears.
class CoinPop : public Coin
{
public:
    CoinPop(double x, double y);

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    double velocityY = -9;
    int frames = 0;
    static const int LIFETIME = 34;     // frames (~0.5 seconds)
};

#endif // COINPOP_H
