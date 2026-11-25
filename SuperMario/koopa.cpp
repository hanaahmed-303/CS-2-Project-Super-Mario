#include "koopa.h"
#include <QTransform>

Koopa::Koopa(double x, double y)
    // The walking Koopa is 48 px tall: start 16 px higher so it stands on the ground
    : Enemy(x, y - SHELL_DROP, QStringLiteral(":/images/koopa.png"))
{
    patrolSpeed = WALK_SPEED;
    health = 20;
    maxHealth = 20;
    contactDamage = 20;
    setPatrolRange(-1e9, 1e9);          // walks until it hits something, like the Goomba

    walkFrame1 = QPixmap(QStringLiteral(":/images/koopa.png"));
    walkFrame2 = QPixmap(QStringLiteral(":/images/koopa_walk2.png"));
    shellFrame = QPixmap(QStringLiteral(":/images/koopa_shell.png"));
}

void Koopa::update()
{
    if (isDead())
        return;

    if (kickGrace > 0)
        kickGrace--;

    if (state == State::Shell) {
        velocityX = 0;                  // a still shell doesn't move
        shellTimer++;
        if (shellTimer > WAKE_UP_FRAMES) {
            // Wakes up and walks again
            state = State::Walking;
            patrolSpeed = WALK_SPEED;
            setY(y() - SHELL_DROP);
            shellTimer = 0;
        }
    } else {
        patrol();                       // Walking or Sliding: move in the current direction
    }

    applyGravity();
    move();

    if (state == State::Walking) {
        animationCounter++;
        showWalkFrame((animationCounter / 12) % 2 == 0 ? walkFrame1 : walkFrame2);
    } else {
        setPixmap(shellFrame);
    }
}

void Koopa::stomp()
{
    switch (state) {
    case State::Walking:
        becomeShell();                  // hides in its shell
        break;
    case State::Sliding:
        state = State::Shell;           // stopping a moving shell
        velocityX = 0;
        shellTimer = 0;
        break;
    case State::Shell:
        kick(direction);                // jumping on a still shell kicks it
        break;
    }
}

bool Koopa::touchedFromSide(const Character *mario)
{
    if (kickGrace > 0)
        return true;                    // just kicked: it can't hurt Mario yet

    if (state == State::Shell) {
        // Kick it away from Mario
        bool marioOnLeft = mario->getBounds().center().x() < getBounds().center().x();
        kick(marioOnLeft ? 1 : -1);
        return true;
    }

    return false;                       // walking Koopa or sliding shell: Mario gets hurt
}

void Koopa::becomeShell()
{
    state = State::Shell;
    velocityX = 0;
    shellTimer = 0;
    setPixmap(shellFrame);
    setY(y() + SHELL_DROP);             // shorter picture: keep it on the ground
}

void Koopa::kick(int dir)
{
    state = State::Sliding;
    direction = dir;
    patrolSpeed = SHELL_SPEED;
    velocityX = SHELL_SPEED * dir;
    kickGrace = 12;
    setPixmap(shellFrame);
}

void Koopa::showWalkFrame(const QPixmap &frame)
{
    // The pictures face left; mirror them when walking right
    if (direction > 0)
        setPixmap(frame.transformed(QTransform().scale(-1, 1)));
    else
        setPixmap(frame);
}
