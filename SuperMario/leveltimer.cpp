#include "leveltimer.h"

LevelTimer::LevelTimer(QObject *parent)
    : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &LevelTimer::tick);
}

void LevelTimer::reset(int startTime)
{
    timer->stop();
    time = startTime;
}

void LevelTimer::resume()
{
    if (time > 0)
        timer->start(TICK_MS);
}

void LevelTimer::pause()
{
    timer->stop();
}

int LevelTimer::timeLeft() const
{
    return time;
}

void LevelTimer::tick()
{
    if (time <= 0)
        return;

    time--;

    if (time == 100)
        emit hurryUp();

    if (time == 0) {
        timer->stop();
        emit timeUp();
    }
}
