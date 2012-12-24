TEMPLATE = app
TARGET = fcardss
TARGET_EXT = .scr
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += static
QMAKE_LFLAGS -= -shared-libgcc
QMAKE_LFLAGS += -static-libgcc

SOURCES += stub.c
OTHER_FILES += \
    ../../../src/platform/fcardsaver.rc \
    ../../../src/platform/fcardsaver.ico

RC_FILE += ../../../src/platform/fcardsaver.rc
OBJECTS_DIR = ../../../build/stub-win32
MOC_DIR = $${OBJECTS_DIR}
RCC_DIR = $${OBJECTS_DIR}
UI_DIR = $${OBJECTS_DIR}
isEmpty(DESTDIR) {
    DESTDIR = $${OBJECTS_DIR}
}
