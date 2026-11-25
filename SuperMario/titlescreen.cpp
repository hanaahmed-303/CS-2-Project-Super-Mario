#include "titlescreen.h"
#include <QFont>

TitleScreen::TitleScreen(QGraphicsItem *parent)
    : QGraphicsPixmapItem(QPixmap(QStringLiteral(":/images/title_logo.png")), parent)
{
    setZValue(95);              // above the level, below the HUD (100)

    // Child item: drawn relative to the logo, so it follows it automatically
    hint = new QGraphicsTextItem(QStringLiteral("CLICK TO START"), this);
    hint->setFont(QFont("Courier New", 20, QFont::Bold));
    hint->setDefaultTextColor(Qt::white);

    double logoWidth = pixmap().width();
    double hintWidth = hint->boundingRect().width();
    hint->setPos((logoWidth - hintWidth) / 2, pixmap().height() + 40);

    hide();
}

void TitleScreen::showAt(double cameraLeft)
{
    setPos(cameraLeft + (SCREEN_WIDTH - pixmap().width()) / 2.0, LOGO_Y);
    show();
}
