#include "soundmanager.h"
#include <QUrl>

SoundManager::SoundManager(QObject *parent)
    : QObject(parent)
{
    // Short sound effects (WAV files), loaded once
    addEffect("jump",     "qrc:/sounds/jump.wav");
    addEffect("coin",     "qrc:/sounds/coin.wav");
    addEffect("stomp",    "qrc:/sounds/stomp.wav");
    addEffect("death",    "qrc:/sounds/death.wav");
    addEffect("gameover", "qrc:/sounds/gameover.wav");
    addEffect("flagpole", "qrc:/sounds/flagpole.wav");
    addEffect("bump",     "qrc:/sounds/bump.wav");
    addEffect("1up",      "qrc:/sounds/1up.wav");
    addEffect("hurryup",  "qrc:/sounds/hurryup.wav");

    // Background music (MP3), repeats forever
    audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(0.4);

    music = new QMediaPlayer(this);
    music->setAudioOutput(audioOutput);
    music->setSource(QUrl("qrc:/sounds/theme.mp3"));
    music->setLoops(QMediaPlayer::Infinite);
}

void SoundManager::addEffect(const QString &name, const QString &file)
{
    QSoundEffect *effect = new QSoundEffect(this);
    effect->setSource(QUrl(file));
    effect->setVolume(0.6);
    effects.insert(name, effect);
}

void SoundManager::play(const QString &name)
{
    if (effects.contains(name))
        effects[name]->play();
}

void SoundManager::playMusic()
{
    music->play();
}

void SoundManager::stopMusic()
{
    music->stop();
}