#include "camera.h"

Camera::Camera(QGraphicsView *view)
    : view(view),
    viewWidth(view->width()),
    viewHeight(view->height()),
    levelWidth(view->width()),
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
    double wantedLeft = targetCenter - viewWidth / 2.0;

    // One-way scrolling: the camera only ever moves forward (right)
    if (wantedLeft > left)
        left = wantedLeft;

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
    // Move the window so its left edge is at "left"
    view->centerOn(left + viewWidth / 2.0, viewHeight / 2.0);
}

void Camera::clampToLevel()
{
    double maxLeft = levelWidth - viewWidth;   // furthest the camera may go
    if (maxLeft < 0)
        maxLeft = 0;                            // level shorter than the screen

    if (left > maxLeft) left = maxLeft;
    if (left < 0)       left = 0;
}