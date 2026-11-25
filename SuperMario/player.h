#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include <QPixmap>
#include <QList>

// Small Mario dies when his health runs out; Big Mario (after a Super Mushroom)
// shrinks back to small when he is hit, instead of losing health.
enum class PlayerShape { SMALL, BIG };

class Player : public Character
{
private:
    double speed = 4.0;
    double jumpStrength = 12.0;

    static const int MAX_LIVES = 3;
    int lives = MAX_LIVES;
    double spawnX;
    double spawnY;

    int damageCooldown = 0;

    PlayerShape shape = PlayerShape::SMALL;

    // ---- Animation ----
    struct Frames {                 // one set of pictures per shape
        QPixmap stand;
        QPixmap jump;
        QPixmap climb;              // holding the flagpole
        QList<QPixmap> walk;
    };
    Frames smallFrames;
    Frames bigFrames;
    QPixmap deadFrame;
    bool facingRight = true;
    int animationCounter = 0;

    const Frames &frames() const;   // the set for the current shape

    void updateAnimation();
    void showFrame(const QPixmap &frame);

public:
    Player(double x, double y, const QString &imagePath);

    void update() override;
    void moveLeft();
    void moveRight();
    void stopMoving();
    void jump();
    void setOnGround(bool grounded) override;

    int getLives() const;
    void loseLife();
    void resetLives();
    void addLife();
    void respawn();
    void setSpawnPoint(double x, double y);

    void takeDamage(int damage) override;
    void showDeathPose();
    void showClimbPose();           // hugging the flagpole

    PlayerShape getShape() const;
    bool grow();                    // Super Mushroom: SMALL -> BIG (false if already big)
    void shrink();                  // hit while BIG: BIG -> SMALL
};

#endif // PLAYER_H
