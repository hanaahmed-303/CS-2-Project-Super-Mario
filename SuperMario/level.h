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
    const QList<QPointF> &getKoopaSpawns() const { return koopaSpawns; }
    const QList<Obstacle *> &getObstacles() const;
    void removeObstacle(Obstacle *obstacle);     // e.g. a broken brick
    const QList<Collectible *> &getCollectibles() const;
    void addCollectible(Collectible *item);    // e.g. a mushroom coming out of a ? block
    Flag *getFlag() const { return root ? flag : nullptr; }
    double getCastleDoorX() const { return castleDoorX; }   // where Mario enters the castle

private:
    QPointer<QGraphicsScene> scene;
    QPointer<QGraphicsWidget> root;
    QRectF bounds;
    QPointF playerSpawn;
    QList<QPointF> enemySpawns;
    QList<QPointF> koopaSpawns;
    QList<Obstacle *> obstacles;
    QList<Collectible *> collectibles;
    Flag *flag = nullptr;
    double castleDoorX = -1;
};

#endif
