QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    animationwindow.cpp \
    bgmcontrol.cpp \
    card.cpp \
    cardpanel.cpp \
    cards.cpp \
    countdown.cpp \
    endingpanel.cpp \
    gamecontrol.cpp \
    gamepanel.cpp \
    loading.cpp \
    main.cpp \
    player.cpp \
    playhand.cpp \
    robot.cpp \
    robotgrablord.cpp \
    robotplayhand.cpp \
    strategy.cpp \
    userplayer.cpp

HEADERS += \
    animationwindow.h \
    bgmcontrol.h \
    card.h \
    cardpanel.h \
    cards.h \
    countdown.h \
    endingpanel.h \
    gamecontrol.h \
    gamepanel.h \
    loading.h \
    player.h \
    playhand.h \
    robot.h \
    robotgrablord.h \
    robotplayhand.h \
    strategy.h \
    userplayer.h

FORMS += \
    gamepanel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
