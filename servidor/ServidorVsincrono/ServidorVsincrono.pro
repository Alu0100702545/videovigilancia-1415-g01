#-------------------------------------------------
#
# Project created by QtCreator 2015-05-02T09:59:41
#
#-------------------------------------------------
PROTOS = VAF.proto
include(protobuf.pri)
QT       += core

QT       += gui

QT       += network

QT       +=sql

TARGET = ServidorV
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11
TEMPLATE = app


SOURCES += main.cpp \
    servidorvsincrono.cpp \
    server.cpp \
    clientet.cpp

DISTFILES += \
    protobuf.pri \
    VAF.proto

HEADERS += \
    servidorvsincrono.h \
    server.h \
    clientet.h
