#include "levelintroscreen.h"
#include <QBrush>
#include <QPen>
#include <QFont>

LevelIntroScreen::LevelIntroScreen(QGraphicsItem *parent)
    : QGraphicsRectItem(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, parent)
{
    setBrush(Qt::black);
    setPen(Qt::NoPen);
    setZValue(90);              // covers the level, but the HUD (100) stays on top

    QFont font("Courier New", 22, QFont::Bold);

    title = new QGraphicsTextItem(this);
    title->setFont(font);
    title->setDefaultTextColor(Qt::white);

    subtitle = new QGraphicsTextItem(this);
    subtitle->setFont(font);
    subtitle->setDefaultTextColor(Qt::white);

    marioIcon = new QGraphicsPixmapItem(QPixmap(QStringLiteral(":/images/mario.png")), this);

    hide();
}

void LevelIntroScreen::showIntro(int level, int lives, double cameraLeft)
{
    showScreen("WORLD 1-" + QString::number(level),
               QString::fromUtf8("×  ") + QString::number(lives),
               true, cameraLeft);
}

void LevelIntroScreen::showTimeUp(double cameraLeft)
{
    showScreen("TIME UP", "", false, cameraLeft);
}

void LevelIntroScreen::showGameOver(double cameraLeft)
{
    showScreen("GAME OVER", "CLICK TO PLAY AGAIN", false, cameraLeft);
}

void LevelIntroScreen::showScreen(const QString &titleText, const QString &subtitleText,
                                  bool withMario, double cameraLeft)
{
    setPos(cameraLeft, 0);      // cover exactly what the window shows

    title->setPlainText(titleText);
    centerText(title, 220);

    subtitle->setPlainText(subtitleText);
    if (withMario) {
        // [Mario]  x  3   -> move the text right to leave room for the icon
        centerText(subtitle, 290, 24);
        marioIcon->setPos(subtitle->x() - 48, 290);
        marioIcon->show();
    } else {
        centerText(subtitle, 300);
        marioIcon->hide();
    }

    show();
}

void LevelIntroScreen::centerText(QGraphicsTextItem *item, double y, double offsetX)
{
    double width = item->boundingRect().width();
    item->setPos((SCREEN_WIDTH - width) / 2 + offsetX, y);
}
