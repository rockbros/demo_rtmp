#-------------------------------------------------
#
# Project created by QtCreator 2015-04-17T21:33:48
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = H264_AAC_RTMP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    audioinfoinput.cpp \
    Active_Object/EventImpl.cpp \
    Active_Object/Task_Base.cpp \
    TimestampBulider.cpp \
    audioencodetask.cpp \
    audioencoderabstract.cpp \
    aacaudioencoderimpl.cpp \
    MOption.cpp \
    core/mglobal.cpp \
    core/mstream.cpp \
    core/mstring.cpp \
    core/mstringlist.cpp \
    videoencodetask.cpp \
    X264EncoderImpl.cpp \
    videocapturetask.cpp \
    avpacket.cpp \
    rgbimage.cpp \
    Convert.cpp \
    RtmpMuxerImpl.cpp \
    rtmpsendtask.cpp

HEADERS  += mainwindow.h \
    audioinfoinput.h \
    Active_Object/EventImpl.h \
    Active_Object/CriticalSection.h \
    Active_Object/Task_Base.h \
    TimestampBulider.h \
    audioencodetask.h \
    audioencoderabstract.h \
    aacaudioencoderimpl.h \
    MOption.h \
    core/mglobal.hpp \
    core/mstream.hpp \
    core/mstring.hpp \
    core/mstringlist.hpp \
    videoencodetask.h \
    X264EncoderImpl.h \
    videocapturetask.h \
    capturetaskpack.h \
    avpacket.h \
    rgbimage.h \
    Convert.h \
    RtmpMuxerImpl.h \
    rtmpsendtask.h

FORMS    += mainwindow.ui

DESTDIR = $$PWD

win32: LIBS += -L$$PWD/library/libfaac/lib/ -lfaac
win32: LIBS += -L$$PWD/library/librtmp/ -lrtmp
win32: LIBS += -L$$PWD/library/libx264/ -lx264
win32: LIBS += -lwinmm -lws2_32  -lz


INCLUDEPATH += $$PWD/core
DEPENDPATH += $$PWD/core

INCLUDEPATH += $$PWD/library/libfaac/include
DEPENDPATH += $$PWD/library/libfaac/include
INCLUDEPATH += $$PWD/library/librtmp
DEPENDPATH += $$PWD/library/librtmp
INCLUDEPATH += $$PWD/library/libx264
DEPENDPATH += $$PWD/library/libx264


