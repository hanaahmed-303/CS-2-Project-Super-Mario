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
#include "titlescreen.h"
#include "levelintroscreen.h"
#include "hud.h"
#include "camera.h"
#include "player.h"
#include "level.h"
#include "collisionmanager.h"
#include "goomba.h"
#include "koopa.h"
#include "collectible.h"
#include "flag.h"
#include "soundmanager.h"
#include "scorepopup.h"
#include "leveltimer.h"
#include "powerup.h"

enum class GameState {
    START_SCREEN,
    LEVEL_INTRO,        // black WORLD 1-1 / TIME UP screen, waiting
    PLAYING,
    PAUSED,
    DYING,
    FLAGPOLE,           // sliding down the pole
    WALK_TO_CASTLE,     // walking into the castle
    TIME_BONUS,         // leftover TIME turns into points
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
    static const int INTRO_MS = 2500;   // how long WORLD 1-1 stays on screen

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
    void updateFlagpole();
    void updateWalkToCastle();
    void updateTimeBonus();
    void addPoints(int points, const QPointF &where);
    void hitBlockAbove();
    void collectCoin();
    void updateDeathAnimation();
    void finishDeath();
    void continueAfterDeath();
    void showLevelIntro();
    double viewLeft();
    QSet<int> keysPressed;
    GameState state;
    MessageScreen *message;
    TitleScreen *title;             // logo before the game starts
    LevelIntroScreen *intro;        // black WORLD / TIME UP / GAME OVER screen
    bool timeRanOut = false;        // the last death was because TIME reached 0
    HUD *hud;
    int levelNumber;
    Camera *camera;
    SoundManager *sound;
    LevelTimer *timer;
    int coins = 0;
    int bonusTime = 0;          // TIME left, counted into points at the end
    Player *player;
    Level *level = nullptr;
    QList<Enemy *> enemies;     // Goombas and Koopas together (polymorphism)
    double deathVelocity = 0;   // Mario's vertical speed during the death animation
    int deathFrames = 0;        // frames since the death animation started
};

#endif // GAME_H