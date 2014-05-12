#-------------------------------------------------
#
# Project created by QtCreator 2014-02-26T09:16:15
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bellatrix
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    src/crc16.c \
    src/buffFunctions.c \
    src/binr2.cpp \
    src/gui_functions.cpp \
    src/port_functions.cpp \
    src/main_settings.cpp \
    src/logging.cpp \
    widgets/infowidget.cpp \
    widgets/config_widget.cpp \
    src/util.cpp \
    src/plugin_manager.cpp \
    src/logToFile.cpp

HEADERS  += mainwindow.h \
    src/typedef.h \
    src/crc16.h \
    src/buffFunctions.h \
    src/binr2.h \
    src/defines.h \
    src/main_settings.h \
    src/logging.h \
    widgets/infowidget.h \
    widgets/config_widget.h \
    src/util.h \
    src/pointersType.h

FORMS    += mainwindow.ui \
    widgets/infowidget.ui \
    widgets/config_widget.ui
INCLUDEPATH += src/
INCLUDEPATH += widgets/

RC_FILE += icon.rc
OTHER_FILES += icon.rc

RESOURCES += \
    img.qrc
