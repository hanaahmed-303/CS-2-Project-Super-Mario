#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>

// The "SUPER MARIO BROS." logo shown over Level 1 before the game starts.
// It IS a picture (inherits QGraphicsPixmapItem) and HAS a text child
// ("CLICK TO START") that moves and hides together with it.
class TitleScreen : public QGraphicsPixmapItem
{
public:
    TitleScreen(QGraphicsItem *parent = nullptr);

    void showAt(double cameraLeft);     // center the logo on the visible screen

private:
    QGraphicsTextItem *hint;

    static const int SCREEN_WIDTH = 800;
    static const int LOGO_Y = 80;
};

#endif // TITLESCREEN_H
