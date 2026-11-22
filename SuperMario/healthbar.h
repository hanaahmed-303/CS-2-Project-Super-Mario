#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <QGraphicsRectItem>

class HealthBar : public QGraphicsRectItem
{
public:
    HealthBar(QGraphicsItem *parent = nullptr);

    void setHealth(int current, int max);

private:
    QGraphicsRectItem *fill;

    static const int BAR_WIDTH  = 150;
    static const int BAR_HEIGHT = 16;
};

#endif // HEALTHBAR_H