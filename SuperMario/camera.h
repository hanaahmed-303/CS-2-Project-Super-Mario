#ifndef CAMERA_H
#define CAMERA_H

#include <QGraphicsView>
#include <QGraphicsItem>

class Camera
{
public:
    Camera(QGraphicsView *view);

    void setLevelWidth(int width);
    void follow(QGraphicsItem *target);
    void reset();
    double getLeft() const;

private:
    void apply();

    QGraphicsView *view;
    int viewWidth;
    int viewHeight;
    int levelWidth;
    double left;
};

#endif // CAMERA_H