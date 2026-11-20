#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

class Game : public QGraphicsView
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);

    static const int WINDOW_WIDTH  = 800;
    static const int WINDOW_HEIGHT = 600;
    static const int FRAME_MS      = 16;

private slots:
    void gameLoop();

private:
    QGraphicsScene *scene;
    QTimer *gameTimer;
};

#endif // GAME_H