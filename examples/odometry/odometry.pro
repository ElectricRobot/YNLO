TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../YNLO/release/ -lYNLO
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../YNLO/debug/ -lYNLO
else:unix: LIBS += -L$$OUT_PWD/../../YNLO/ -lYNLO

INCLUDEPATH += $$PWD/../../YNLO
DEPENDPATH += $$PWD/../../YNLO

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../YNLO/release/libYNLO.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../YNLO/debug/libYNLO.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../YNLO/release/YNLO.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../YNLO/debug/YNLO.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../YNLO/libYNLO.a

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv
