#include "hud.h"
#include <QFont>

HUD::HUD(QGraphicsScene *scene)
{
    // Create the pieces
    score = new Score();
    scene->addItem(score);

    healthBar = new HealthBar();
    scene->addItem(healthBar);

    livesText = createText(scene);
    levelText = createText(scene);

    // Starting values and layout
    refresh(3, 3, 3, 1);
    placeItems(0, 0);
}

void HUD::refresh(int health, int maxHealth, int lives, int level)
{
    healthBar->setHealth(health, maxHealth);
    livesText->setPlainText("Lives: " + QString::number(lives));
    levelText->setPlainText("Level " + QString::number(level));
}

Score *HUD::getScore() const
{
    return score;
}

QGraphicsTextItem *HUD::createText(QGraphicsScene *scene)
{
    QGraphicsTextItem *text = new QGraphicsTextItem();
    text->setFont(QFont("Arial", 16, QFont::Bold));
    text->setDefaultTextColor(Qt::black);
    text->setZValue(100);
    scene->addItem(text);
    return text;
}

void HUD::placeItems(double left, double top)
{
    score->setPos(left + 10, top + 5);
    healthBar->setPos(left + 12, top + 38);
    levelText->setPos(left + 350, top + 5);
    livesText->setPos(left + 680, top + 5);
}