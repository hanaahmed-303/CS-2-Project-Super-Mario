#include "hud.h"
#include <QFont>
#include <QPixmap>

HUD::HUD(QGraphicsScene *scene)
{
    score = new Score();
    scene->addItem(score);

    healthBar = new HealthBar();
    scene->addItem(healthBar);

    marioTitle = createText(scene, "MARIO");
    worldTitle = createText(scene, "WORLD");
    timeTitle  = createText(scene, "TIME");
    livesTitle = createText(scene, "LIVES");

    // Small coin picture next to the coin counter
    QPixmap coin(":/images/coin.png");
    coinIcon = new QGraphicsPixmapItem(
        coin.scaled(coin.width() / 2, coin.height() / 2,
                    Qt::KeepAspectRatio, Qt::FastTransformation));
    coinIcon->setZValue(100);
    scene->addItem(coinIcon);

    coinText  = createText(scene);
    levelText = createText(scene);
    timeText  = createText(scene);
    livesText = createText(scene);

    refresh(3, 3, 3, 1, 0, 400);
    placeItems(0, 0);
}

void HUD::refresh(int health, int maxHealth, int lives, int level, int coins, int time)
{
    healthBar->setHealth(health, maxHealth);
    coinText->setPlainText(QString("x%1").arg(coins, 2, 10, QChar('0')));
    levelText->setPlainText(QString("1-%1").arg(level));
    timeText->setPlainText(QString("%1").arg(time, 3, 10, QChar('0')));
    livesText->setPlainText(QString("x%1").arg(lives));
}

Score *HUD::getScore() const
{
    return score;
}

QGraphicsTextItem *HUD::createText(QGraphicsScene *scene, const QString &text)
{
    QGraphicsTextItem *item = new QGraphicsTextItem(text);
    item->setFont(QFont("Courier New", 16, QFont::Bold));
    item->setDefaultTextColor(Qt::white);
    item->setZValue(100);
    scene->addItem(item);
    return item;
}

void HUD::placeItems(double left, double top)
{
    // Row 1: titles
    marioTitle->setPos(left + 40,  top + 4);
    worldTitle->setPos(left + 380, top + 4);
    timeTitle->setPos(left + 540,  top + 4);
    livesTitle->setPos(left + 680, top + 4);

    // Row 2: values
    score->setPos(left + 40,       top + 26);
    coinIcon->setPos(left + 230,   top + 32);
    coinText->setPos(left + 244,   top + 26);
    levelText->setPos(left + 392,  top + 26);
    timeText->setPos(left + 548,   top + 26);
    livesText->setPos(left + 692,  top + 26);

    // Row 3: health bar (required by the project spec)
    healthBar->setPos(left + 42,   top + 54);
}

void HUD::followCamera(double viewLeft)
{
    placeItems(viewLeft, 0);
}
