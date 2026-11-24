#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QAudioOutput>

class SoundManager : public QObject
{
    Q_OBJECT

public:
    explicit SoundManager(QObject *parent = nullptr);

    void play(const QString &name);    // play a short sound effect
    void playMusic();                  // start the background music (loops)
    void stopMusic();                  // stop the background music

private:
    void addEffect(const QString &name, const QString &file);

    QMap<QString, QSoundEffect *> effects;   // name → sound
    QMediaPlayer *music;
    QAudioOutput *audioOutput;
};

#endif // SOUNDMANAGER_H