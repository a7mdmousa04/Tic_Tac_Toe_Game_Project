QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

INCLUDEPATH += $$PWD/../Header-files_include

TEMPLATE = app

SOURCES +=  \
    test_gamelogic.cpp \
    $$PWD/../Source-code_scr/gamelogic.cpp

HEADERS += \
    test_gamelogic.h \
    $$PWD/../Header-files_include/gamelogic.h
