#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QSet>
#include <QMouseEvent>
#include "messagescreen.h"
#include "hud.h"

enum class GameState {
    START_SCREEN,
    PLAYING,
    PAUSED,
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

private:
    QGraphicsScene *scene;
    QTimer *gameTimer;
    void handleInput();
    QSet<int> keysPressed;
    GameState state;
    MessageScreen *message;
    HUD *hud;
    int levelNumber;
};

#endif // GAME_H