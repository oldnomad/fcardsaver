#-------------------------------------------------
#
# Project created by QtCreator 2012-08-30T16:42:18
#
#-------------------------------------------------

QT       += core gui

TARGET = fcardsaver
TEMPLATE = app

SOURCES += main.cpp\
    saverwindow.cpp \
    saversettings.cpp \
    card/card.cpp \
    card/cardcell.cpp \
    card/cardset.cpp \
    card/carddisplay.cpp \
    setup/setupdialog.cpp \
    setup/setupfilesmodel.cpp \
    setup/setupsplitmodel.cpp \
    setup/setupsplitdelegate.cpp \
    setup/setupcelldialog.cpp \
    util/cmdoption.cpp \
    util/textconv.cpp \
    util/colorbutton.cpp \
    util/csvfile.cpp \
    platform/platform-win32.cpp \
    platform/platform-x11.cpp

HEADERS  += \
    saverwindow.h \
    saversettings.h \
    card/card.h \
    card/cardcell.h \
    card/cardset.h \
    card/carddisplay.h \
    setup/setupdialog.h \
    setup/setupfilesmodel.h \
    setup/setupsplitmodel.h \
    setup/setupsplitcell.h \
    setup/setupsplitdelegate.h \
    setup/setupcelldialog.h \
    util/cmdoption.h \
    util/textconv.h \
    util/colorbutton.h \
    util/csvfile.h \
    platform/startup.h \
    platform/nativeroot.h

FORMS    += \
    setup/setupdialog.ui \
    setup/setupcelldialog.ui

OTHER_FILES += \
    platform/fcardsaver.rc \
    platform/fcardsaver.ico

INCLUDEPATH += .
CONFIG(debug, debug|release) {
    MY_TARGET = debug
} else:CONFIG(release, debug|release) {
    MY_TARGET = release
    DEFINES += QT_NO_DEBUG_OUTPUT
}
QMAKE_LINK_OBJECT_MAX = 32
windows {
    LIBS += -lshlwapi
    RC_FILE += platform/fcardsaver.rc
    QMAKE_MAKEFILE = Makefile_win32
    OBJECTS_DIR = ../build/$${MY_TARGET}-win32
} else:unix {
    LIBS += -lX11
    QMAKE_MAKEFILE = Makefile_linux
    OBJECTS_DIR = ../build/$${MY_TARGET}-linux
}
MOC_DIR = $${OBJECTS_DIR}
RCC_DIR = $${OBJECTS_DIR}
UI_DIR = $${OBJECTS_DIR}
isEmpty(DESTDIR) {
    DESTDIR = $${OBJECTS_DIR}
}
unix {
    binary.files = $${OBJECTS_DIR}/fcardsaver
    binary.path = /usr/bin/
    INSTALLS += binary
}
