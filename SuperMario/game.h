#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QSet>

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

    static const int WINDOW_WIDTH  = 800;
    static const int WINDOW_HEIGHT = 600;
    static const int FRAME_MS      = 16;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;


private slots:
    void gameLoop();

private:
    QGraphicsScene *scene;
    QTimer *gameTimer;
    void handleInput();
    QSet<int> keysPressed;
    GameState state;
};

#endif // GAME_H