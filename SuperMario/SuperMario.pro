QT += widgets multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    background.cpp \
    collectible.cpp \
    collisionmanager.cpp \
    flag.cpp \
    level.cpp \
    leveltimer.cpp \
    obstacle.cpp \
    camera.cpp \
    character.cpp \
    enemy.cpp \
    game.cpp \
    gameobject.cpp \
    goomba.cpp \
    healthbar.cpp \
    hud.cpp \
    koopa.cpp \
    main.cpp \
    messagescreen.cpp \
    player.cpp \
    powerup.cpp \
    score.cpp \
    scorepopup.cpp \
    soundmanager.cpp

HEADERS += \
    background.h \
    collectible.h \
    collisionmanager.h \
    flag.h \
    level.h \
    leveltimer.h \
    obstacle.h \
    camera.h \
    character.h \
    enemy.h \
    game.h \
    gameobject.h \
    goomba.h \
    healthbar.h \
    hud.h \
    koopa.h \
    messagescreen.h \
    player.h \
    powerup.h \
    score.h \
    scorepopup.h \
    soundmanager.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resouces.qrc
