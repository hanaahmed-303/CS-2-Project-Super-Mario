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

#endif
