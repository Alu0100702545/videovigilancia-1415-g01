#-------------------------------------------------
#
# Project created by QtCreator 2015-04-07T13:28:24
#
#-------------------------------------------------


PROTOS = VAF.proto
include(protobuf.pri)
QT       += core

QT       -= gui
QT       += network
TARGET = ServidorV
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    servidor.cpp

HEADERS += \
    servidor.h

OTHER_FILES += \
    VAF.proto \
    protobuf.pri


