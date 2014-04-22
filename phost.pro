#-------------------------------------------------
#
# Project created by QtCreator 2014-04-17T13:27:59
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = phost
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    cvideoplay.cpp \
    cnetwork.cpp

HEADERS  += widget.h \
    cvideoplay.h \
    constvaule.h \
    cnetwork.h \
    utils.h

FORMS    += widget.ui \
    cvideoplay.ui

OTHER_FILES += \
    images/volume.png \
    images/video-display.png \
    images/video.png \
    images/stop.png \
    images/skipForward.png \
    images/skipBackward.png \
    images/seekForward.png \
    images/seekBackward.png \
    images/reload.png \
    images/play.png \
    images/pause.png \
    images/mute.png \
    images/eject.xcf \
    images/eject.png \
    images/audio.png \
    images/audio.ico \
    images/addFile.png

RESOURCES += \
    player.qrc
