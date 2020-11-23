#-------------------------------------------------
#
# Project created by QtCreator 2017-05-09T01:11:17
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = jiankong
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    publicfile.cpp \
    setting.cpp \
    screen.cpp \
    record.cpp \
    ffmpeg.cpp \
    decoder.cpp \
    screenthread.cpp \
    recordthread.cpp \
    showpicture.cpp \
    playvideo.cpp \
    logindialog.cpp

HEADERS  += mainwindow.h \
    publicfile.h \
    setting.h \
    screen.h \
    record.h \
    ffmpeg.h \
    decoder.h \
    screenthread.h \
    recordthread.h \
    externdata.h \
    showpicture.h \
    playvideo.h \
    logindialog.h \
    yuyvtorgb888.h

FORMS    += mainwindow.ui \
    setting.ui \
    screen.ui \
    record.ui \
    showpicture.ui \
    playvideo.ui

INCLUDEPATH +=  ./ffmpeg/include
LIBS += -L./ffmpeg/lib/ -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswscale -lswresample


QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable
QMAKE_CXXFLAGS += -Wno-write-strings
QMAKE_CXXFLAGS += -Wno-deprecated-declarations
QMAKE_CXXFLAGS += -Wno-sign-compare
