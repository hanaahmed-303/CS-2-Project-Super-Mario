#include "camera.h"

Camera::Camera(QGraphicsView *view)
    : view(view),
    levelWidth(0),
    left(0)
{
}

void Camera::setLevelWidth(int width)
{
    levelWidth = width;
}

void Camera::follow(QGraphicsItem *target)
{
    if (!target)
        return;

    double targetCenter = target->sceneBoundingRect().center().x();
    double wantedLeft = targetCenter - view->width() / 2.0;


    // One-way scrolling: the camera only ever moves forward (right)
    if (wantedLeft > left)
        left = wantedLeft;

    clampToLevel();
    apply();
}

void Camera::reset()
{
    left = 0;
    apply();
}

double Camera::getLeft() const
{
    return left;
}

void Camera::apply()
{
    view->centerOn(left + view->width() / 2.0, view->height() / 2.0);
}

void Camera::clampToLevel()
{
    double maxLeft = levelWidth - view->width();  // furthest the camera may go
    if (maxLeft < 0)
        maxLeft = 0;                            // level shorter than the screen

    if (left > maxLeft) left = maxLeft;
    if (left < 0)       left = 0;
}