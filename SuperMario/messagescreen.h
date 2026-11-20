#ifndef MESSAGESCREEN_H
#define MESSAGESCREEN_H

#include <QGraphicsTextItem>
#include <QString>

class MessageScreen : public QGraphicsTextItem
{
public:
    MessageScreen(QGraphicsItem *parent = nullptr);

    void showMessage(const QString &text);
    void hideMessage();

private:
    void centerOnScreen();
};

#endif // MESSAGESCREEN_H