#include "gameobject.h"
#include <QPixmap>
#include <QDebug>

GameObject::GameObject(double x, double y, const QString &imagePath,
                       QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent)
{
    QPixmap image(imagePath);
    if (image.isNull())
        qDebug() << "GameObject: image not found:" << imagePath;

    setPixmap(image);
    width  = image.width();
    height = image.height();
    setPos(x, y);
}

QRectF GameObject::getBounds() const
{
    return sceneBoundingRect();
}