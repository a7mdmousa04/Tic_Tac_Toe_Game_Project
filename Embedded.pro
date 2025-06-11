QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += $$PWD/Header-files_include

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += Header-files_include/gamelogic.h \
           Header-files_include/mainwindow.h \
           Header-files_include/userauth.h

SOURCES += Source-code_scr/gamelogic.cpp \
           Source-code_scr/main.cpp \
           Source-code_scr/mainwindow.cpp \
           Source-code_scr/userauth.cpp

FORMS += ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
