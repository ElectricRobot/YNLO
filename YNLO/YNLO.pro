#-------------------------------------------------
#
# Project created by QtCreator 2017-05-01T11:13:48
#
#-------------------------------------------------

QT       -= core gui

TARGET = YNLO
TEMPLATE = lib
CONFIG += staticlib
# for c++14 flags
CONFIG += c++14
QMAKE_CXXFLAGS += -std=gnu++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    UdpSocket.cpp \
    Except.cpp \
    Pipeline.cpp \
    cv/ORBExtractor.cpp

HEADERS += \
    UdpSocket.h \
    Except.h \
    Pipeline.h \
    Buffer.h \
    cv/CamParam.h \
    cv/SparseFeature.h \
    cv/ORBExtractor.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -pthread

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv
