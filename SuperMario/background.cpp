#include "background.h"

#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>

Background::Background(const QSize &size, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    QPixmap sky(size);
    QPainter painter(&sky);
    painter.setRenderHint(QPainter::Antialiasing);
    QLinearGradient gradient(0, 0, 0, size.height());
    gradient.setColorAt(0, QColor("#73b9d8"));
    gradient.setColorAt(1, QColor("#f4ecd1"));
    painter.fillRect(sky.rect(), gradient);
    painter.setPen(Qt::NoPen);

    for (int x = 0; x < size.width(); x += 560) {
        painter.setBrush(QColor("#e5f3ee"));
        painter.drawEllipse(x + 85, 145, 115, 26);
        painter.drawEllipse(x + 110, 130, 65, 36);
        painter.drawEllipse(x + 365, 210, 82, 20);
        painter.drawEllipse(x + 386, 197, 42, 29);

        const int bottom = size.height();
        QPainterPath ridge;
        ridge.moveTo(x, bottom);
        ridge.lineTo(x, bottom - 83);
        ridge.cubicTo(x + 150, bottom - 83, x + 165, bottom - 211,
                      x + 280, bottom - 211);
        ridge.cubicTo(x + 400, bottom - 211, x + 410, bottom - 83,
                      x + 560, bottom - 83);
        ridge.lineTo(x + 560, bottom);
        ridge.closeSubpath();
        painter.fillPath(ridge, QColor("#8cb6a5"));
    }

    painter.end();
    setPixmap(sky);
    setZValue(-100);
}
