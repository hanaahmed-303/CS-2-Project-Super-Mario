#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QSet>
#include <QMouseEvent>
#include <QList>
#include "messagescreen.h"
#include "hud.h"
#include "camera.h"
#include "player.h"
#include "level.h"
#include "collisionmanager.h"
#include "goomba.h"
#include "collectible.h"
#include "flag.h"
#include "soundmanager.h"

enum class GameState {
    START_SCREEN,
    PLAYING,
    PAUSED,
    DYING,
    LEVEL_COMPLETE,
    GAME_OVER,
    VICTORY
};

class Game : public QGraphicsView
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    void startGame();
    void restartLevel();
    void gameOver();
    void nextLevel();
    void winGame();
    void exitGame();

    static const int WINDOW_WIDTH  = 800;
    static const int WINDOW_HEIGHT = 600;
    static const int FRAME_MS      = 16;
    static const int MAX_LEVELS = 3;

    ~Game();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;


private slots:
    void gameLoop();
    void updateCamera();
    void handlePlayerDeath();
    void resetGame();

private:
    QGraphicsScene *scene;
    QTimer *gameTimer;
    void handleInput();
    bool loadLevel(int n);
    void spawnEnemies();
    void clearEnemies();
    void levelComplete();
    void updateDeathAnimation();
    void finishDeath();
    QSet<int> keysPressed;
    GameState state;
    MessageScreen *message;
    HUD *hud;
    int levelNumber;
    Camera *camera;
    SoundManager *sound;
    Player *player;
    Level *level = nullptr;
    QList<Goomba *> enemies;
    double deathVelocity = 0;   // Mario's vertical speed during the death animation
    int deathFrames = 0;        // frames since the death animation started
};

#endif // GAME_H