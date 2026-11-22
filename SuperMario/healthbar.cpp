#include "healthbar.h"
#include <QBrush>
#include <QPen>
#include <QColor>

HealthBar::HealthBar(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    // Background = the "empty" part of the bar
    setRect(0, 0, BAR_WIDTH, BAR_HEIGHT);
    setBrush(QColor(60, 60, 60));          // dark grey
    setPen(QPen(Qt::black, 2));            // black border
    setZValue(100);                        // on top of the game

    // Fill = the part that shrinks. A CHILD of the background.
    fill = new QGraphicsRectItem(this);
    fill->setPen(Qt::NoPen);

    setHealth(1, 1);                       // start full
}

void HealthBar::setHealth(int current, int max)
{
    if (max <= 0)
        return;

    // Keep current between 0 and max
    if (current < 0)   current = 0;
    if (current > max) current = max;

    double ratio = static_cast<double>(current) / max;   // 0.0 to 1.0

    fill->setRect(0, 0, BAR_WIDTH * ratio, BAR_HEIGHT);

    // Color changes as health drops
    if (ratio > 0.5)
        fill->setBrush(Qt::green);
    else if (ratio > 0.25)
        fill->setBrush(QColor(255, 200, 0));   // yellow
    else
        fill->setBrush(Qt::red);
}