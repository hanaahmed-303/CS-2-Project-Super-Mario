#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QGraphicsPixmapItem>
#include <QList>
#include <QPointF>
#include <QString>

// One piece of scenery painted into the background (cloud, bush, hill, castle)
struct SceneryItem
{
    QString imagePath;
    QPointF bottomLeft;     // where the picture's bottom-left corner goes
};

class Background : public QGraphicsPixmapItem
{
public:
    Background(const QSize &size, const QList<SceneryItem> &scenery,
               QGraphicsItem *parent = nullptr);
};

#endif
