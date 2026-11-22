#include "game.h"

Game::Game(QWidget *parent) : QGraphicsView(parent)
{
    state = GameState::START_SCREEN;
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    setScene(scene);

    levelNumber = 1;

    // HUD: score, health bar, lives, level (always on screen)
    hud = new HUD(scene);

    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWindowTitle("Super Mario");
    // Camera (must come AFTER setFixedSize, it reads the window size)
    camera = new Camera(this);

    // ---- TEMPORARY test world ----
    scene->setSceneRect(0, 0, 3000, WINDOW_HEIGHT);
    camera->setLevelWidth(3000);

    for (int x = 0; x < 3000; x += 200)          // markers to see movement
        scene->addRect(x, 500, 40, 40);

    testTarget = scene->addRect(400, 450, 32, 48, QPen(Qt::black), QBrush(Qt::red));
    // ---- end TEMPORARY ----

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
Game::~Game()
{
    delete hud;
    delete camera;

}
void Game::gameLoop()
{
    if (state != GameState::PLAYING)
        return;

    handleInput();
    updateCamera();

    // Later: update player & enemies, check collisions,
    // update HUD, move camera.
}

void Game::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;
    keysPressed.insert(event->key());
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

    // TEMPORARY: arrow keys move the test square
    if (right) testTarget->moveBy(5, 0);
    if (left)  testTarget->moveBy(-5, 0);

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
void Game::updateCamera()
{
    camera->follow(testTarget);              // TEMPORARY - later: camera->follow(player);
    hud->followCamera(camera->getLeft());
}
