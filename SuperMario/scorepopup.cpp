#include "scorepopup.h"
#include <QFont>
#include <QTimerEvent>

ScorePopup::ScorePopup(int points, const QPointF &position)
{
    setPlainText(QString::number(points));
    setFont(QFont("Arial", 12, QFont::Bold));
    setDefaultTextColor(Qt::white);
    setZValue(50);                  // above the level and Mario, below the HUD
    setPos(position);

    startTimer(16);                 // its own little animation, ~60 times a second
}

void ScorePopup::timerEvent(QTimerEvent *event)
{
    frames++;

    moveBy(0, -1.5);                // float upwards

    // Fade out during the second half of its life
    if (frames > LIFETIME / 2)
        setOpacity(1.0 - double(frames - LIFETIME / 2) / (LIFETIME / 2));

    if (frames >= LIFETIME) {
        killTimer(event->timerId());
        deleteLater();              // remove itself safely from the scene
    }
}
