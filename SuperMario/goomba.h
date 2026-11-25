#ifndef GOOMBA_H
#define GOOMBA_H

#include "enemy.h"
#include <QPixmap>

class Goomba : public Enemy
{
private:
    QPixmap walkFrame1;
    QPixmap walkFrame2;
    QPixmap flatFrame;
    int animationCounter = 0;

public:
    Goomba(double x, double y, const QString &imagePath);

    void update() override;
    void stomp();
    void vanish();      // removed without the squash (fell into a pit)
};

#endif
