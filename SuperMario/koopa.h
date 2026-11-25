#ifndef KOOPA_H
#define KOOPA_H

#include "enemy.h"
#include <QPixmap>

// Koopa Troopa: a turtle that walks like a Goomba.
//  - stomp it while walking  -> it hides in its shell (still)
//  - touch / stomp the shell -> Mario kicks it, it slides fast
//  - a sliding shell knocks out other enemies and hurts Mario
//  - a shell left alone for 5 seconds wakes up and walks again
class Koopa : public Enemy
{
public:
    Koopa(double x, double y);

    void update() override;
    void stomp() override;
    bool touchedFromSide(const Character *mario) override;

    bool isSlidingShell() const { return state == State::Sliding; }

private:
    enum class State { Walking, Shell, Sliding };
    State state = State::Walking;

    QPixmap walkFrame1;
    QPixmap walkFrame2;
    QPixmap shellFrame;
    int animationCounter = 0;
    int shellTimer = 0;         // frames spent as a still shell
    int kickGrace = 0;          // frames right after a kick when touching Mario is harmless

    void becomeShell();
    void kick(int dir);
    void showWalkFrame(const QPixmap &frame);

    static constexpr double WALK_SPEED  = 1.0;
    static constexpr double SHELL_SPEED = 7.0;
    static const int WAKE_UP_FRAMES = 300;      // ~5 seconds
    static const int SHELL_DROP = 16;           // the shell is 16 px shorter than the walking Koopa
};

#endif // KOOPA_H
