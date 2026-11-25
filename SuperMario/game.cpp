#include "game.h"
#include <utility>

Game::Game(QWidget *parent) : QGraphicsView(parent)
{
    state = GameState::START_SCREEN;
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    setScene(scene);

    levelNumber = 1;

    // HUD: score, health bar, lives, level (always on screen)
    hud = new HUD(scene);

    // Sound: music + effects
    sound = new SoundManager(this);

    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWindowTitle("Super Mario");
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    // Camera (must come AFTER setFixedSize, it reads the window size)
    camera = new Camera(this);


    // Mario
    player = new Player(100, 300, ":/images/mario.png");
    player->setZValue(10);          // above blocks, below HUD (100)
    scene->addItem(player);
    // TEMPORARY floor with a pit (visual only)


    // Message in the middle of the screen
    message = new MessageScreen();
    scene->addItem(message);
    message->showMessage("Click to start");

    // Load the first level (creates the world and places Mario)
    loadLevel(levelNumber);

    // 4. Game loop timer
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &Game::gameLoop);
    gameTimer->start(FRAME_MS);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}
Game::~Game()
{
    clearEnemies();
    delete level;
    delete hud;
    delete camera;

}
void Game::gameLoop()
{
    if (state != GameState::PLAYING)
        return;

    // 1. Moving platforms move first (so they can carry Mario this frame)
    level->updateAll();

    // 2. Remember where Mario was, then let him move
    QRectF previousBounds = player->getBounds();
    handleInput();
    player->update();

    // 3. Collisions with the level (Mohamed's CollisionManager)
    CollisionManager::resolveObstacles(player, previousBounds, level->getObstacles());
    CollisionManager::checkHazards(player, level->getObstacles());

    // Enemies: move, collide with blocks, then check against Mario
    for (Goomba *goomba : std::as_const(enemies)) {        if (goomba->isDead())
            continue;

        QRectF goombaPrevious = goomba->getBounds();
        goomba->update();
        CollisionManager::resolveObstacles(goomba, goombaPrevious, level->getObstacles());

        if (CollisionManager::checkEnemyCollision(player, goomba, previousBounds)) {
            hud->getScore()->increase(100);
            sound->play("stomp");
        }
    }

    // Coins: touching one adds its points to the score
    for (Collectible *item : level->getCollectibles()) {
        if (item->isCollected())
            continue;

        if (player->getBounds().intersects(item->getBounds())) {
            hud->getScore()->increase(item->collect());
            sound->play("coin");
        }
    }

    // 4. Invisible walls: screen's left edge, and the end of the level
    if (player->x() < camera->getLeft())
        player->setX(camera->getLeft());

    double levelRight = level->getBounds().right();
    double marioWidth = player->boundingRect().width();
    if (player->x() + marioWidth > levelRight)
        player->setX(levelRight - marioWidth);

    // 5. Death: fell below the level, or health reached 0
    if (player->y() > level->getBounds().bottom() || player->isDead()) {
        handlePlayerDeath();
        return;
    }

    // 6. Camera + HUD
    updateCamera();
    hud->refresh(player->getHealth(), player->getMaxHealth(),
                 player->getLives(), levelNumber);
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
    // Horizontal movement
    if (left)
        player->moveLeft();
    else if (right)
        player->moveRight();
    else
        player->stopMoving();

    if (jump && player->isOnGround()) {
        player->jump();
        sound->play("jump");
    }
}

void Game::mousePressEvent(QMouseEvent *event)
{
    if (state == GameState::START_SCREEN) {
        startGame();
    }
    else if (state == GameState::GAME_OVER) {
        resetGame();
    }
    else if (state == GameState::VICTORY) {
        exitGame();
    }

    QGraphicsView::mousePressEvent(event);
}

void Game::startGame()
{
    message->hideMessage();
    state = GameState::PLAYING;
    setFocus();
    sound->playMusic();          // ← NEW: background music
}
void Game::updateCamera()
{
    camera->follow(player);
    hud->followCamera(mapToScene(0, 0).x());
}


void Game::handlePlayerDeath()
{
    player->loseLife();

    if (player->getLives() == 0) {
        sound->stopMusic();
        sound->play("gameover");
        gameOver();
        return;
    }

    sound->play("death");
    restartLevel();
}
void Game::restartLevel()
{
    // Rebuild the current level: coins and Goombas come back,
    // Mario returns to the start with full health, camera resets.
    loadLevel(levelNumber);
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

    loadLevel(levelNumber);     // rebuild Level 1 from scratch
    startGame();                // hide the message, state = PLAYING
}

bool Game::loadLevel(int n)
{
    clearEnemies();
    delete level;
    level = new Level(scene);

    QString error;
    QString path = QString(":/levels/level%1.txt").arg(n);
    if (!level->loadFromFile(path, &error)) {
        qDebug() << "Could not load level" << n << ":" << error;
        return false;                                  // ← changed
    }

    camera->setLevelWidth(level->getBounds().width());
    QPointF spawn = level->getPlayerSpawn();
    player->setSpawnPoint(spawn.x(), spawn.y());
    player->respawn();
    camera->reset();

    spawnEnemies();
    return true;                                       // ← new
}

void Game::spawnEnemies()
{
    for (const QPointF &pos : level->getEnemySpawns()) {
        Goomba *goomba = new Goomba(pos.x(), pos.y(), ":/images/goomba.png");
        goomba->setZValue(5);          // above blocks, below Mario (10)
        scene->addItem(goomba);
        enemies.append(goomba);
    }
}

void Game::clearEnemies()
{
    qDeleteAll(enemies);   // delete every Goomba (also removes it from the scene)
    enemies.clear();       // empty the list
}

void Game::levelComplete()
{
    state = GameState::LEVEL_COMPLETE;     // freeze the game
    hud->getScore()->increase(500);        // bonus for finishing the level
    hud->refresh(player->getHealth(), player->getMaxHealth(),
                 player->getLives(), levelNumber);

    message->showMessage("LEVEL " + QString::number(levelNumber) + " COMPLETE!");

    // Wait 2 seconds, then go to the next level
    QTimer::singleShot(2000, this, &Game::nextLevel);
}

void Game::nextLevel()
{
    levelNumber++;

    // No more levels (or the next file doesn't exist yet) → the player has won
    if (levelNumber > MAX_LEVELS || !loadLevel(levelNumber)) {
        winGame();
        return;
    }

    keysPressed.clear();     // don't keep running from the previous level
    startGame();             // hide the message, state = PLAYING
}

void Game::winGame()
{
    state = GameState::VICTORY;
    message->showMessage("CONGRATULATIONS!\nYou finished the game\nScore: "
                         + QString::number(hud->getScore()->getScore())
                         + "\nClick to exit");
}

void Game::exitGame()
{
    close();      // closes the window → the program ends
}