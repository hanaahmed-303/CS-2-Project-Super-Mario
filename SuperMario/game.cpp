#include "game.h"

// TEMPORARY floor height until Level has real ground
static const double TEMP_GROUND_Y = 500;
static const double TEMP_LEVEL_WIDTH = 3000;
static const double TEMP_PIT_START = 1200;
static const double TEMP_PIT_END   = 1350;

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
    // TEMPORARY floor with a pit (visual only)
    scene->addRect(0, TEMP_GROUND_Y, TEMP_PIT_START, 20,
                   QPen(Qt::black), QBrush(Qt::darkGreen));
    scene->addRect(TEMP_PIT_END, TEMP_GROUND_Y, TEMP_LEVEL_WIDTH - TEMP_PIT_END, 20,
                   QPen(Qt::black), QBrush(Qt::darkGreen));

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

    // TEMPORARY floor check (with a pit) until Level has real ground + collisions
    double marioHeight = player->boundingRect().height();
    double marioFeet   = player->y() + marioHeight;
    double marioCenterX = player->x() + player->boundingRect().width() / 2;
    bool overPit = marioCenterX > TEMP_PIT_START && marioCenterX < TEMP_PIT_END;

    if (!overPit && marioFeet >= TEMP_GROUND_Y && marioFeet <= TEMP_GROUND_Y + 20) {
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

    // Death: fell into a pit, or health reached 0
    if (player->y() > WINDOW_HEIGHT || player->isDead()) {
        handlePlayerDeath();
        return;     // skip the rest of this frame
    }

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
    else if (state == GameState::GAME_OVER) {
        resetGame();
    }
    // Later:
    // else if (state == GameState::VICTORY) { exitGame(); }

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


void Game::handlePlayerDeath()
{
    player->loseLife();

    if (player->getLives() == 0) {
        gameOver();
        return;
    }

    restartLevel();
}
void Game::restartLevel()
{
    // Later (with Level): reload the level so enemies and coins come back
    player->respawn();      // back to start, full health, short invincibility
    camera->reset();        // camera back to the beginning of the level
}


void Game::gameOver()
{
    state = GameState::GAME_OVER;      // freezes the game loop

    // Show the final numbers (the loop is frozen, so refresh once here)
    hud->refresh(player->getHealth(), player->getMaxHealth(),
                 player->getLives(), levelNumber);

    message->showMessage("GAME OVER\nClick to restart");
}

void Game::resetGame()
{
    levelNumber = 1;
    hud->getScore()->reset();   // the ONLY place the score is reset
    player->resetLives();

    restartLevel();             // Later: loadLevel(1) instead
    startGame();                // hide the message, state = PLAYING
}