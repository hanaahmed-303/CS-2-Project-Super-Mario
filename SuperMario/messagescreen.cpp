#include "messagescreen.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFont>

MessageScreen::MessageScreen(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    setFont(QFont("Arial", 28, QFont::Bold));
    setDefaultTextColor(Qt::black);
    setZValue(100);      // always drawn on top of everything else
    hide();              // invisible until showMessage() is called
}

void MessageScreen::showMessage(const QString &text)
{
    setPlainText(text);
    centerOnScreen();
    show();
}

void MessageScreen::hideMessage()
{
    hide();
}

void MessageScreen::centerOnScreen()
{
    if (!scene() || scene()->views().isEmpty())
        return;

    // The middle of what the window is currently showing
    QGraphicsView *view = scene()->views().first();
    QPointF screenCenter = view->mapToScene(view->rect().center());

    // Move the text so ITS center sits on the screen's center
    QRectF box = boundingRect();
    setPos(screenCenter.x() - box.width() / 2,
           screenCenter.y() - box.height() / 2);
}