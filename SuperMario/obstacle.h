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
};

class Pipe : public StaticObstacle
{
public:
    Pipe(double x, double y);
    bool isHazard() const override { return false; }
};

class QuestionBlock : public StaticObstacle
{
public:
    QuestionBlock(double x, double y);
    bool isHazard() const override { return false; }
    bool isUsed() const { return used; }
    bool hit();          // true only the first time (it gives a coin once)

private:
    bool used = false;
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
