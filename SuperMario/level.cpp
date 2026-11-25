#include "level.h"

#include "background.h"
#include "collectible.h"
#include "flag.h"
#include "obstacle.h"

#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <memory>
#include <utility>

Level::Level(QGraphicsScene *scene)
    : scene(scene)
{
}

Level::~Level()
{
    delete root.data();
}

bool Level::loadFromFile(const QString &path, QString *error)
{
    const auto fail = [error](const QString &message) {
        if (error)
            *error = message;
        return false;
    };

    if (!scene)
        return fail(QStringLiteral("No scene available for the level."));

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return fail(QStringLiteral("%1: %2").arg(path, file.errorString()));

    QTextStream input(&file);
    QStringList rows;
    int columns = 0;
    while (!input.atEnd()) {
        const QString row = input.readLine();
        columns = std::max(columns, static_cast<int>(row.size()));
        rows.append(row);
    }
    if (input.status() != QTextStream::Ok)
        return fail(QStringLiteral("Could not read %1.").arg(path));

    auto newRoot = std::make_unique<QGraphicsWidget>();
    newRoot->setZValue(-1);  // Characters added directly to the scene draw above the level.
    QList<Obstacle *> newObstacles;
    QList<Collectible *> newCollectibles;
    QList<QPointF> newEnemySpawns;
    Flag *newFlag = nullptr;
    QPointF newSpawn;
    bool hasSpawn = false;

    for (int row = 0; row < rows.size(); ++row) {
        for (int column = 0; column < rows[row].size(); ++column) {
            const int x = column * TILE_SIZE;
            const int y = row * TILE_SIZE;
            Obstacle *obstacle = nullptr;

            switch (rows[row][column].unicode()) {
            case '.':
            case ' ':
                break;
            case '#':
                obstacle = new Ground(x, y);
                break;
            case 'B':
                obstacle = new Brick(x, y);
                break;
            case 'Q':
                obstacle = new QuestionBlock(x, y);
                break;
            case 'P':
                obstacle = new Pipe(x, y);
                break;
            case 'S':
                obstacle = new Spike(x, y);
                break;
            case 'M':
                obstacle = new MovingPlatform(x, y);
                break;
            case 'C': {
                auto *coin = new Coin(x, y);
                coin->setParentItem(newRoot.get());
                newCollectibles.append(coin);
                break;
            }
            case 'G':
                newEnemySpawns.append(QPointF(x, y));
                break;
            case 'A':
                if (hasSpawn)
                    return fail(QStringLiteral("The map has more than one player spawn (A)."));
                hasSpawn = true;
                newSpawn = QPointF(x, y);
                break;
            case 'F':
                if (newFlag)
                    return fail(QStringLiteral("The map has more than one flag (F)."));
                newFlag = new Flag(x, y);
                newFlag->setParentItem(newRoot.get());
                break;
            default:
                return fail(QStringLiteral("Unknown tile '%1' at row %2, column %3.")
                                .arg(rows[row][column]).arg(row + 1).arg(column + 1));
            }

            if (obstacle) {
                obstacle->setParentItem(newRoot.get());
                newObstacles.append(obstacle);
            }
        }
    }

    if (!hasSpawn || !newFlag)
        return fail(QStringLiteral("The map needs one player spawn (A) and one flag (F)."));

    const QSize size(columns * TILE_SIZE, rows.size() * TILE_SIZE);
    new Background(size, newRoot.get());

    delete root.data();
    root = newRoot.release();
    scene->addItem(root.data());
    bounds = QRectF(QPointF(0, 0), QSizeF(size));
    scene->setSceneRect(bounds);
    playerSpawn = newSpawn;
    enemySpawns = std::move(newEnemySpawns);
    obstacles = std::move(newObstacles);
    collectibles = std::move(newCollectibles);
    flag = newFlag;
    if (error)
        error->clear();
    return true;
}

const QList<Obstacle *> &Level::getObstacles() const
{
    static const QList<Obstacle *> empty;
    return root ? obstacles : empty;
}

const QList<Collectible *> &Level::getCollectibles() const
{
    static const QList<Collectible *> empty;
    return root ? collectibles : empty;
}

void Level::updateAll()
{
    for (Obstacle *obstacle : getObstacles())
        obstacle->update();
}
