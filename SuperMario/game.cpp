#include "game.h"
#include <QGraphicsPixmapItem>
#include <QDebug>

Game::Game(QWidget *parent) : QGraphicsView(parent)
{
    state = GameState::START_SCREEN;
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

    // Message in the middle of the screen
    message = new MessageScreen();
    scene->addItem(message);
    message->showMessage("Click to start");


    // 4. Game loop timer
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &Game::gameLoop);
    gameTimer->start(FRAME_MS);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

void Game::gameLoop()
{
    if (state != GameState::PLAYING)
        return;

    handleInput();
    static int frame = 0;
    frame++;
    if (frame % 60 == 0)
        qDebug() << "Game loop running, frame" << frame;

    // Later: update player & enemies, check collisions,
    // update HUD, move camera.
}
void Game::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;
    keysPressed.insert(event->key());
    qDebug() << "Pressed:" << event->key();   // TEMPORARY test
}

void Game::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;
    keysPressed.remove(event->key());
}

void Game::handleInput()
{
    bool left  = keysPressed.contains(Qt::Key_Left)  || keysPressed.contains(Qt::Key_A);
    bool right = keysPressed.contains(Qt::Key_Right) || keysPressed.contains(Qt::Key_D);
    bool jump  = keysPressed.contains(Qt::Key_Space) || keysPressed.contains(Qt::Key_Up)
                || keysPressed.contains(Qt::Key_W);

    // Later: if (left) player->moveLeft(); else if (right) player->moveRight();
    //        else player->stop();  if (jump) player->jump();

    Q_UNUSED(left); Q_UNUSED(right); Q_UNUSED(jump);
}

void Game::mousePressEvent(QMouseEvent *event)
{
    if (state == GameState::START_SCREEN) {
        startGame();
    }
    // Later:
    // else if (state == GameState::GAME_OVER) { restart from Level 1 }
    // else if (state == GameState::VICTORY)   { exitGame(); }

    QGraphicsView::mousePressEvent(event);
}

void Game::startGame()
{
    message->hideMessage();
    state = GameState::PLAYING;
    setFocus();        // make sure the keyboard works right after clicking
}
