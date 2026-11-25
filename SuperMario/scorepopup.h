#ifndef SCOREPOPUP_H
#define SCOREPOPUP_H

#include <QGraphicsTextItem>

// A number (like "100") that floats up from where points were earned,
// fades out, and then deletes itself - like in the original game.
class ScorePopup : public QGraphicsTextItem
{
public:
    ScorePopup(int points, const QPointF &position);

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    int frames = 0;
    static const int LIFETIME = 45;     // frames (~0.75 seconds)
};

#endif // SCOREPOPUP_H
