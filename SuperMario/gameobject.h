#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QString>

class GameObject : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    GameObject(double x, double y, const QString &imagePath,
               QGraphicsItem *parent = nullptr);
    virtual ~GameObject() = default;

    virtual void update() = 0;
    QRectF getBounds() const;       // its rectangle in the scene, for collisions

protected:
    int width;
    int height;
};

#endif // GAMEOBJECT_H