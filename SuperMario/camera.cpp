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

    // Where is the middle of the target in the world?
    double targetCenter = target->sceneBoundingRect().center().x();

    // Put the target in the middle of the screen
    left = targetCenter - viewWidth / 2.0;

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