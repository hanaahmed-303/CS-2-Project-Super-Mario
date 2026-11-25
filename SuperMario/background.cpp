#include "background.h"

#include <QPainter>
#include <QPixmap>

Background::Background(const QSize &size, const QList<SceneryItem> &scenery,
                       QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    // Classic Super Mario sky blue
    QPixmap picture(size);
    picture.fill(QColor(92, 148, 252));

    // Paint every cloud, bush, hill and the castle onto the sky
    QPainter painter(&picture);
    for (const SceneryItem &item : scenery) {
        QPixmap image(item.imagePath);
        painter.drawPixmap(QPointF(item.bottomLeft.x(),
                                   item.bottomLeft.y() - image.height()),
                           image);
    }
    painter.end();

    setPixmap(picture);
    setZValue(-100);        // behind everything else in the level
}
