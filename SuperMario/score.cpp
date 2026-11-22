#include "score.h"
#include <QFont>

Score::Score(QGraphicsItem *parent)
    : QGraphicsTextItem(parent), score(0)
{
    setFont(QFont("Arial", 16, QFont::Bold));
    setDefaultTextColor(Qt::black);
    setZValue(100);      // drawn on top of the game
    updateText();
}

void Score::increase(int points)
{
    score += points;
    updateText();
}

void Score::reset()
{
    score = 0;
    updateText();
}

int Score::getScore() const
{
    return score;
}

void Score::updateText()
{
    setPlainText("Score: " + QString::number(score));
}