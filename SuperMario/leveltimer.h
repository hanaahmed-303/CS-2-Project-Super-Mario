#ifndef LEVELTIMER_H
#define LEVELTIMER_H

#include <QObject>
#include <QTimer>

// The "TIME" countdown of a level, like in the original game.
// It sends a signal at 100 (hurry up!) and at 0 (time up -> Mario dies).
class LevelTimer : public QObject
{
    Q_OBJECT

public:
    explicit LevelTimer(QObject *parent = nullptr);

    void reset(int startTime);     // set the time, stopped
    void resume();                 // start / continue counting
    void pause();                  // stop counting (death, level complete...)
    int timeLeft() const;

signals:
    void hurryUp();                // reached 100
    void timeUp();                 // reached 0

private slots:
    void tick();

private:
    QTimer *timer;
    int time = 0;

    static const int TICK_MS = 400;   // one "TIME" unit, like the NES (faster than a second)
};

#endif // LEVELTIMER_H
