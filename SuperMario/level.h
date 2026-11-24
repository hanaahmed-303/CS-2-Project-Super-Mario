#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QList>
#include <QPointer>

class Collectible;
class Flag;
class Obstacle;

class Level
{
public:
    static constexpr int TILE_SIZE = 32;

    explicit Level(QGraphicsScene *scene);
    ~Level();

    Level(const Level &) = delete;
    Level &operator=(const Level &) = delete;

    bool loadFromFile(const QString &path, QString *error = nullptr);
    void updateAll();

    QRectF getBounds() const { return bounds; }
    QPointF getPlayerSpawn() const { return playerSpawn; }
    const QList<QPointF> &getEnemySpawns() const { return enemySpawns; }
    const QList<Obstacle *> &getObstacles() const;
    const QList<Collectible *> &getCollectibles() const;
    Flag *getFlag() const { return root ? flag : nullptr; }

private:
    QPointer<QGraphicsScene> scene;
    QPointer<QGraphicsWidget> root;
    QRectF bounds;
    QPointF playerSpawn;
    QList<QPointF> enemySpawns;
    QList<Obstacle *> obstacles;
    QList<Collectible *> collectibles;
    Flag *flag = nullptr;
};

#endif
