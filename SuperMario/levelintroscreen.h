#ifndef LEVELINTROSCREEN_H
#define LEVELINTROSCREEN_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>

// The black full-window screen from the original game, used for:
//   WORLD 1-1  (Mario x 3)   before every level and after losing a life
//   TIME UP                  when the timer reached 0
//   GAME OVER                when all lives are gone
// The HUD (z = 100) stays visible on top of it, like in the original.
class LevelIntroScreen : public QGraphicsRectItem
{
public:
    LevelIntroScreen(QGraphicsItem *parent = nullptr);

    void showIntro(int level, int lives, double cameraLeft);
    void showTimeUp(double cameraLeft);
    void showGameOver(double cameraLeft);

private:
    QGraphicsTextItem *title;       // "WORLD 1-1", "TIME UP", "GAME OVER"
    QGraphicsTextItem *subtitle;    // "x  3" or "CLICK TO PLAY AGAIN"
    QGraphicsPixmapItem *marioIcon; // small Mario next to the lives

    void showScreen(const QString &titleText, const QString &subtitleText,
                    bool withMario, double cameraLeft);
    void centerText(QGraphicsTextItem *item, double y, double offsetX = 0);

    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 600;
};

#endif // LEVELINTROSCREEN_H
