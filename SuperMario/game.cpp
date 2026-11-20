#include "game.h"
#include <QGraphicsPixmapItem>
#include <QDebug>

Game::Game(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    setScene(scene);

    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWindowTitle("Super Mario");

    // 3. TEMPORARY test that resources work (delete later)
    QPixmap testImage(":/images/mario.png");
    if (testImage.isNull())
        qDebug() << "Image NOT found - check resources.qrc";
    QGraphicsPixmapItem *test = scene->addPixmap(testImage);
    test->setPos(100, 400);

    // 4. Game loop timer
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &Game::gameLoop);
    gameTimer->start(FRAME_MS);
}

void Game::gameLoop()
{
    // TEMPORARY test: print once per second to prove the loop runs
    static int frame = 0;
    frame++;
    if (frame % 60 == 0)
        qDebug() << "Game loop running, frame" << frame;

    // Later: update player & enemies, check collisions,
    // update HUD, move camera.
}
