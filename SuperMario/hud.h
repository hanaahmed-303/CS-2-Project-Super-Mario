#ifndef HUD_H
#define HUD_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "score.h"
#include "healthbar.h"

class HUD
{
public:
    HUD(QGraphicsScene *scene);

    void refresh(int health, int maxHealth, int lives, int level);
    Score *getScore() const;

private:
    QGraphicsTextItem *createText(QGraphicsScene *scene);
    void placeItems(double left, double top);

    Score *score;
    HealthBar *healthBar;
    QGraphicsTextItem *livesText;
    QGraphicsTextItem *levelText;
};

#endif // HUD_H