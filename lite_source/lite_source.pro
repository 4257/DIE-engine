QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = diel

XCONFIG += use_dex
XCONFIG += use_archive

macx {
    TARGET = DiEL
}

include(../build.pri)

SOURCES += \
    litemainwindow.cpp \
    main_lite.cpp

HEADERS += \
    litemainwindow.h

FORMS += \
    litemainwindow.ui

!contains(XCONFIG, die_script) {
    XCONFIG += die_script
    include(../die_script/die_script.pri)
}

!contains(XCONFIG, xoptions) {
    XCONFIG += xoptions
    include(../XOptions/xoptions.pri)
}

win32 {
    RC_ICONS = ../icons/lite.ico
}

macx {
    ICON = ../icons/lite.icns
}

DISTFILES += \
    CMakeLists.txt
