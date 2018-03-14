#-------------------------------------------------
#
# Project created by QtCreator 2016-07-14T20:32:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ncDocument
TEMPLATE = app


SOURCES += ./src/main.cpp\
        ./src/widget.cpp \
    ./src/ncdocument.cpp \
    ./src/ncRunBase.cpp \
    ./src/ncRunFanucMill.cpp \
    ./src/ncRunFanucLathe.cpp \
    ./src/ncDocISOGcode.cpp \

HEADERS  += ./inc/widget.h \
    ./inc/ncdocument.h \
    ./inc/ncRunBase.h \
    ./inc/ncRunFanucMill.h \
    ./inc/common.h \
    ./inc/fanuc.h \
    ./inc/ncRunFanucLathe.h \
    ./inc/ncDocISOGcode.h \

FORMS    += ./ui/widget.ui

DESTDIR += ../bin
