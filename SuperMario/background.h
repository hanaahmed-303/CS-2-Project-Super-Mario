#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QGraphicsPixmapItem>

class Background : public QGraphicsPixmapItem
{
public:
    explicit Background(const QSize &size, QGraphicsItem *parent = nullptr);
};

#endif
