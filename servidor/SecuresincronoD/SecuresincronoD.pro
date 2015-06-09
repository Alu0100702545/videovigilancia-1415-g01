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


unix {
 # Variables
 #
 isEmpty(PREFIX) {
 PREFIX = /usr/local
 }

 BINDIR = $$PREFIX/bin
 DATADIR = $$PREFIX/share
 CONFDIR = /etc

 isEmpty(VARDIR) {
 VARDIR = /var/lib/$${TARGET}
 }

 DEFINES += APP_DATADIR="$$DATADIR"
 DEFINES += APP_VARDIR=\\\"$$VARDIR\\\"
 DEFINES += APP_CONFFILE=\\\"$$CONFDIR/$${TARGET}.ini\\\"
 DEFINES += APP_CERTCLA=\\\"$$CONFDIR/$${TARGET}\\\"
 # Install
 #
 INSTALLS += target config desktop vardir
 ## Instalar ejecutable
 target.path = $$BINDIR

 ## Instalar archivo de configuración
 config.path = $$CONFDIR
 config.files += $${TARGET}.ini

 ## Instalar acceso directo en el menú del escritorio
 desktop.path = $$DATADIR/applications
 desktop.files += $${TARGET}.desktop

 ## Instalar icono de aplicación
 ##icon32.path = $$DATADIR/icons/hicolor/32x32/apps
 ##icon32.files += ./data/32x32/$${TARGET}.png

 ## Crear directorio de archivos variables
 vardir.path = $$VARDIR
 #vardir.extra = mkdir -p $$VARDIR/clientes && mkdir -p $$VARDIR/BDD
 vardir.commands = true
}
