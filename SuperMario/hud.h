#ifndef HUD_H
#define HUD_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include "score.h"
#include "healthbar.h"

// The info bar at the top of the screen, laid out like the original game:
//   MARIO       (coin)       WORLD     TIME     LIVES
//   000000      x00          1-1       400      x3
//   [health bar]
class HUD
{
public:
    HUD(QGraphicsScene *scene);

    void refresh(int health, int maxHealth, int lives, int level,
                 int coins = 0, int time = 0);
    Score *getScore() const;
    void followCamera(double viewLeft);

private:
    QGraphicsTextItem *createText(QGraphicsScene *scene, const QString &text = QString());
    void placeItems(double left, double top);

    Score *score;
    HealthBar *healthBar;

    // Titles (top row)
    QGraphicsTextItem *marioTitle;
    QGraphicsTextItem *worldTitle;
    QGraphicsTextItem *timeTitle;
    QGraphicsTextItem *livesTitle;

    // Values (second row)
    QGraphicsPixmapItem *coinIcon;
    QGraphicsTextItem *coinText;
    QGraphicsTextItem *levelText;
    QGraphicsTextItem *timeText;
    QGraphicsTextItem *livesText;
};

#endif // HUD_H
