#include "game.h"

// TEMPORARY floor height until Level has real ground
static const double TEMP_GROUND_Y = 500;
static const double TEMP_LEVEL_WIDTH = 3000;

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

    // TEMPORARY level size until Level exists (later: done in loadLevel)
    scene->setSceneRect(0, 0, TEMP_LEVEL_WIDTH, WINDOW_HEIGHT);
    camera->setLevelWidth(TEMP_LEVEL_WIDTH);

    // Mario
    player = new Player(100, 300, ":/images/mario.png");
    player->setZValue(10);          // above blocks, below HUD (100)
    scene->addItem(player);
    // TEMPORARY floor + markers (visual only)
    scene->addRect(0, TEMP_GROUND_Y, TEMP_LEVEL_WIDTH, 20, QPen(Qt::black), QBrush(Qt::darkGreen));
    for (int x = 200; x < TEMP_LEVEL_WIDTH; x += 300)
        scene->addRect(x, TEMP_GROUND_Y - 40, 40, 40, QPen(Qt::black), QBrush(Qt::gray));

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
    player->update();

    // TEMPORARY floor check until Level has real ground + collisions
    double marioHeight = player->boundingRect().height();
    if (player->y() + marioHeight >= TEMP_GROUND_Y) {
        player->setY(TEMP_GROUND_Y - marioHeight);   // stand exactly on top
        player->setOnGround(true);
    } else {
        player->setOnGround(false);
    }

    // Invisible wall at the left edge of the screen (one-way scrolling)
    if (player->x() < camera->getLeft())
        player->setX(camera->getLeft());

    // TEMPORARY wall at the end of the level (later: the flag ends the level)
    double marioWidth = player->boundingRect().width();
    if (player->x() + marioWidth > TEMP_LEVEL_WIDTH)
        player->setX(TEMP_LEVEL_WIDTH - marioWidth);

    updateCamera();

    // Show Mario's current health, lives and the level number
    hud->refresh(player->getHealth(), player->getMaxHealth(),
                 player->getLives(), levelNumber);
}

void Game::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;
    keysPressed.insert(event->key());
    // TEMPORARY test: press H to take damage


    if (event->key() == Qt::Key_H)
        player->takeDamage(25);
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
    // Horizontal movement
    if (left)
        player->moveLeft();
    else if (right)
        player->moveRight();
    else
        player->stopMoving();

    // Jump
    if (jump)
        player->jump();
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
    camera->follow(player);

    // Where is the window REALLY showing right now?
    double viewLeft = mapToScene(0, 0).x();
    hud->followCamera(viewLeft);

    // TEMPORARY debug: compare the camera's idea with reality
    static int frame = 0;
    if (++frame % 60 == 0)
        qDebug() << "camera left:" << camera->getLeft()
                 << "| real view left:" << viewLeft
                 << "| sceneRect:" << scene->sceneRect();
}
