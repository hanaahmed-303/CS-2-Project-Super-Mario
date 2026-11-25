#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "gameobject.h"

class Obstacle : public GameObject
{
public:
    using GameObject::GameObject;
};

class StaticObstacle : public Obstacle
{
public:
    using Obstacle::Obstacle;
    void update() override {}
    virtual bool isHazard() const = 0;
};

class MovingObstacle : public Obstacle
{
public:
    using Obstacle::Obstacle;
    virtual QPointF getDisplacement() const = 0;
};

class Ground : public StaticObstacle
{
public:
    Ground(double x, double y);
    bool isHazard() const override { return false; }
};

class Brick : public StaticObstacle
{
public:
    Brick(double x, double y);
    bool isHazard() const override { return false; }
    void bump();        // small Mario hits it: it bounces but doesn't break
};

class Pipe : public StaticObstacle
{
public:
    // isTop: the rim at the top of the pipe; isLeft: the left half of the pipe
    Pipe(double x, double y, bool isTop, bool isLeft);
    bool isHazard() const override { return false; }
};

class QuestionBlock : public StaticObstacle
{
public:
    enum class Content { Coin, Mushroom };      // what comes out when hit

    QuestionBlock(double x, double y, Content content = Content::Coin);
    bool isHazard() const override { return false; }
    bool isUsed() const { return used; }
    Content getContent() const { return content; }
    bool hit();          // true only the first time (it gives its content once)

private:
    bool used = false;
    Content content;
};

class HardBlock : public StaticObstacle
{
public:
    HardBlock(double x, double y);      // staircase block, can't be broken
    bool isHazard() const override { return false; }
};

class Spike : public StaticObstacle
{
public:
    Spike(double x, double y);
    bool isHazard() const override { return true; }
};

class MovingPlatform : public MovingObstacle
{
public:
    MovingPlatform(double x, double y);
    void update() override;
    QPointF getDisplacement() const override { return displacement; }

private:
    double startX;
    double speed = 2;
    QPointF displacement;
};

#endif
