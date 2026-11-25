#include "game.h"
#include <algorithm>
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

    // TIME countdown: hurry-up sound at 100, Mario dies at 0
    timer = new LevelTimer(this);
    connect(timer, &LevelTimer::hurryUp, this, [this]() { sound->play("hurryup"); });
    connect(timer, &LevelTimer::timeUp, this, [this]() {
        if (state == GameState::PLAYING)
            handlePlayerDeath();
    });

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
    // Mario's death animation runs on its own; everything else is frozen
    if (state == GameState::DYING) {
        updateDeathAnimation();
        return;
    }

    // End of the level: each step runs on its own
    if (state == GameState::FLAGPOLE) {
        updateFlagpole();
        return;
    }
    if (state == GameState::WALK_TO_CASTLE) {
        updateWalkToCastle();
        return;
    }
    if (state == GameState::TIME_BONUS) {
        updateTimeBonus();
        return;
    }

    if (state != GameState::PLAYING)
        return;

    PlayerShape shapeBefore = player->getShape();

    // 1. Moving platforms move first (so they can carry Mario this frame)
    level->updateAll();

    // 2. Remember where Mario was, then let him move
    QRectF previousBounds = player->getBounds();
    handleInput();
    player->update();

    // 3. Collisions with the level (Mohamed's CollisionManager)
    double speedYBefore = player->getVelocityY();
    CollisionManager::resolveObstacles(player, previousBounds, level->getObstacles());

    // Mario was going up and the collision stopped him -> he hit a block with his head
    if (speedYBefore < 0 && player->getVelocityY() == 0)
        hitBlockAbove();
    CollisionManager::checkHazards(player, level->getObstacles());

    // Enemies: move, collide with blocks, then check against Mario
    // Like the original game, an enemy only starts walking once it
    // comes onto the screen (so it doesn't wander off before Mario arrives)
    double screenRight = camera->getLeft() + WINDOW_WIDTH;

    for (Enemy *enemy : std::as_const(enemies)) {
        if (enemy->isDead())
            continue;

        if (enemy->x() > screenRight + 32)
            continue;                       // still waiting off-screen

        QRectF enemyPrevious = enemy->getBounds();
        enemy->update();

        double speedBefore = enemy->getVelocityX();
        CollisionManager::resolveObstacles(enemy, enemyPrevious, level->getObstacles());

        // Walked into a block or pipe (collision stopped it) -> turn around
        // (a sliding Koopa shell bounces off walls the same way)
        if (speedBefore != 0 && enemy->getVelocityX() == 0)
            enemy->reverseDirection();

        // Spikes are walls for enemies too (CollisionManager only hurts Mario with them)
        for (Obstacle *obstacle : level->getObstacles()) {
            StaticObstacle *spike = dynamic_cast<StaticObstacle *>(obstacle);
            if (spike && spike->isHazard() &&
                enemy->getBounds().intersects(spike->getBounds())) {
                enemy->setX(enemy->x() - (enemy->getBounds().left() - enemyPrevious.left()));
                enemy->reverseDirection();
                break;
            }
        }

        // Start of the level is a wall
        if (enemy->x() < 0) {
            enemy->setX(0);
            enemy->reverseDirection();
        }

        // Fell into a pit -> gone
        if (enemy->y() > level->getBounds().bottom()) {
            enemy->vanish();
            continue;
        }

        // Koopa: was it a still shell before Mario touched it? (then touching = kick)
        Koopa *koopa = dynamic_cast<Koopa *>(enemy);
        bool wasSliding = koopa && koopa->isSlidingShell();

        if (CollisionManager::checkEnemyCollision(player, enemy, previousBounds)) {
            addPoints(100, enemy->pos() + QPointF(0, -16));
            sound->play(koopa ? "kick" : "stomp");
        } else if (koopa && !wasSliding && koopa->isSlidingShell()) {
            // Mario kicked a still shell from the side
            addPoints(400, enemy->pos() + QPointF(0, -16));
            sound->play("kick");
        }
    }

    // Enemies touching each other
    for (int i = 0; i < enemies.size(); ++i) {
        for (int j = i + 1; j < enemies.size(); ++j) {
            Enemy *a = enemies[i];
            Enemy *b = enemies[j];
            if (a->isDead() || b->isDead())
                continue;
            if (!a->getBounds().intersects(b->getBounds()))
                continue;

            // A sliding Koopa shell knocks out whatever it hits
            Koopa *shellA = dynamic_cast<Koopa *>(a);
            Koopa *shellB = dynamic_cast<Koopa *>(b);
            bool aSliding = shellA && shellA->isSlidingShell();
            bool bSliding = shellB && shellB->isSlidingShell();

            if (aSliding || bSliding) {
                Enemy *victim = aSliding ? b : a;
                if (aSliding && bSliding)
                    victim = b;             // two shells: just knock one out
                addPoints(500, victim->pos() + QPointF(0, -16));
                sound->play("kick");
                victim->vanish();
                continue;
            }

            // Otherwise two enemies walking into each other both turn around
            bool aIsLeft = a->x() < b->x();
            bool approaching = (aIsLeft && a->getVelocityX() > 0) ||
                               (!aIsLeft && a->getVelocityX() < 0);
            if (approaching) {
                a->reverseDirection();
                b->reverseDirection();
            }
        }
    }

    // Collectibles: coins and power-ups
    for (Collectible *item : level->getCollectibles()) {
        if (item->isCollected() || !item->isVisible())
            continue;

        // Power-ups move by themselves (rise out of the block, then slide)
        PowerUp *powerUp = dynamic_cast<PowerUp *>(item);
        if (powerUp) {
            powerUp->move(level->getObstacles(), level->getBounds().bottom());
            if (powerUp->isEmerging())
                continue;                   // can't be taken while coming out
        }

        if (player->getBounds().intersects(item->getBounds())) {
            addPoints(item->collect(), item->scenePos());
            if (powerUp) {
                powerUp->applyTo(player);   // e.g. Mushroom -> Big Mario
                sound->play("powerup");
            } else {
                sound->play("coin");
                collectCoin();
            }
        }
    }

    // Big Mario was hit and shrank this frame
    if (shapeBefore == PlayerShape::BIG && player->getShape() == PlayerShape::SMALL)
        sound->play("powerdown");

    // Flag: touching it finishes the level
    Flag *flag = level->getFlag();
    if (flag && player->getBounds().intersects(flag->getBounds()) && flag->activate()) {
        levelComplete();
        return;
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
                 player->getLives(), levelNumber,
                 coins, timer->timeLeft());
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
    timer->resume();            // TIME starts counting
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
    timer->pause();
    state = GameState::DYING;   // freeze the level, only Mario moves
    keysPressed.clear();

    sound->stopMusic();
    sound->play("death");

    player->showDeathPose();
    deathVelocity = -10;        // the little hop up
    deathFrames = 0;
}

void Game::updateDeathAnimation()
{
    deathFrames++;

    // Short pause in the dead pose (0.5 s), then hop up and fall slowly
    if (deathFrames > 30) {
        player->moveBy(0, deathVelocity);
        deathVelocity += 0.35;          // weaker than normal gravity = slower fall
    }

    // When the death sound is over (~2.8 s), continue the game
    if (deathFrames > 170)
        finishDeath();
}

void Game::finishDeath()
{
    player->loseLife();

    if (player->getLives() == 0) {
        sound->play("gameover");
        gameOver();
        return;
    }

    restartLevel();     // rebuild the level, Mario back at the start
    startGame();        // state = PLAYING, music starts again
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
                 player->getLives(), levelNumber,
                 coins, timer->timeLeft());

    message->showMessage("GAME OVER\nClick to restart");
}

void Game::resetGame()
{
    coins = 0;
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
    timer->reset(400);          // every level (and every retry) starts with TIME 400
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

    for (const QPointF &pos : level->getKoopaSpawns()) {
        Koopa *koopa = new Koopa(pos.x(), pos.y());
        koopa->setZValue(5);
        scene->addItem(koopa);
        enemies.append(koopa);
    }
}

void Game::clearEnemies()
{
    qDeleteAll(enemies);   // delete every enemy (also removes it from the scene)
    enemies.clear();       // empty the list
}

void Game::levelComplete()
{
    // Mario grabbed the flagpole
    timer->pause();
    sound->stopMusic();
    sound->play("flagpole");
    keysPressed.clear();
    state = GameState::FLAGPOLE;

    Flag *flag = level->getFlag();

    // The higher he grabs the pole, the more points (like the original)
    double height = (flag->getPoleBottom() - player->getBounds().bottom())
                    / (flag->getPoleBottom() - flag->getPoleTop());
    int points = height > 0.8 ? 5000 : height > 0.6 ? 2000 :
                 height > 0.4 ? 800  : height > 0.2 ? 400  : 100;
    addPoints(points, QPointF(flag->getPoleX() + 12, player->y()));

    // Hug the pole (Mario on its left side)
    player->setVelocityX(0);
    player->setVelocityY(0);
    player->setX(flag->getPoleX() - player->boundingRect().width() + 4);
    player->showClimbPose();
}

void Game::updateFlagpole()
{
    Flag *flag = level->getFlag();

    // The flag and Mario both slide down
    bool flagDown = flag->lowerCloth(4);

    double bottom = flag->getPoleBottom();
    double feet = player->getBounds().bottom();
    if (feet < bottom)
        player->moveBy(0, std::min(4.0, bottom - feet));
    bool marioDown = player->getBounds().bottom() >= bottom;

    if (flagDown && marioDown) {
        // Hop to the other side of the pole and walk to the castle
        player->setX(flag->getPoleX() + 4);
        state = GameState::WALK_TO_CASTLE;
    }

    updateCamera();
}

void Game::updateWalkToCastle()
{
    QRectF previousBounds = player->getBounds();
    player->moveRight();
    player->update();
    CollisionManager::resolveObstacles(player, previousBounds, level->getObstacles());

    // Reached the castle door -> Mario goes inside
    if (player->getBounds().center().x() >= level->getCastleDoorX()) {
        player->hide();
        bonusTime = timer->timeLeft();
        state = GameState::TIME_BONUS;
    }

    updateCamera();
}

void Game::updateTimeBonus()
{
    if (bonusTime > 0) {
        // Every unit of TIME left is worth 50 points, counted down quickly
        int step = std::min(bonusTime, 2);
        bonusTime -= step;
        hud->getScore()->increase(50 * step);
        if (bonusTime % 10 == 0)
            sound->play("beep");

        hud->refresh(player->getHealth(), player->getMaxHealth(),
                     player->getLives(), levelNumber, coins, bonusTime);
        return;
    }

    // Done: show the message, then the next level after 2 seconds
    state = GameState::LEVEL_COMPLETE;
    message->showMessage("LEVEL " + QString::number(levelNumber) + " COMPLETE!");
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
    sound->stopMusic();
    state = GameState::VICTORY;
    message->showMessage("CONGRATULATIONS!\nYou finished the game\nScore: "
                         + QString::number(hud->getScore()->getScore())
                         + "\nClick to exit");
}

void Game::exitGame()
{
    close();      // closes the window → the program ends
}

void Game::addPoints(int points, const QPointF &where)
{
    hud->getScore()->increase(points);             // add to the score
    scene->addItem(new ScorePopup(points, where));  // show the floating number
}

void Game::hitBlockAbove()
{
    // A point just above the middle of Mario's head
    QRectF mario = player->getBounds();
    QPointF probe(mario.center().x(), mario.top() - 2);

    for (Obstacle *obstacle : level->getObstacles()) {
        if (!obstacle->getBounds().contains(probe))
            continue;

        QuestionBlock *block = dynamic_cast<QuestionBlock *>(obstacle);
        if (block && block->hit()) {
            if (block->getContent() == QuestionBlock::Content::Mushroom) {
                // A Super Mushroom rises out of the block (block bounced up 8 px)
                Mushroom *mushroom = new Mushroom(block->x(), block->y() + 8);
                level->addCollectible(mushroom);
                sound->play("powerup_appears");
            } else {
                sound->play("coin");
                addPoints(200, block->scenePos() + QPointF(4, -28));
                collectCoin();
            }
        } else {
            sound->play("bump");
        }
        return;
    }

    sound->play("bump");    // hit the edge of a block
}

void Game::collectCoin()
{
    coins++;

    // Like the original: every 100 coins gives an extra life
    if (coins >= 100) {
        coins = 0;
        player->addLife();
        sound->play("1up");
    }
}
