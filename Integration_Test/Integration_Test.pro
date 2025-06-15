QT       += core gui
QT       += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += testcase

INCLUDEPATH += $$PWD/../Header-files_include
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/../Source-code_scr/gamelogic.cpp \
    integration_tests.cpp \
    $$PWD/../Source-code_scr/mainwindow.cpp \
    $$PWD/../Source-code_scr/userauth.cpp

HEADERS += \
    $$PWD/../Header-files_include/gamelogic.h \
    $$PWD/../Header-files_include/mainwindow.h \
    $$PWD/../Header-files_include/userauth.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
