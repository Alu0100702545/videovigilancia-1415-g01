#-------------------------------------------------
#
# Project created by QtCreator 2015-04-22T08:08:22
#
#-------------------------------------------------
include(protobuf.pri)
PROTOS = VAF.proto

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(QtOpenCV.pri)


QT       += core
QT       += multimedia multimediawidgets
QT       -= gui

TARGET = clienteCLI
CONFIG   += console
CONFIG += c++11
CONFIG   -= app_bundle

TEMPLATE = app


add_opencv_modules(core video imgproc)

SOURCES += main.cpp\
        clientecli.cpp

HEADERS  += clientecli.h \
    capturebuffer.h

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
    DEFINES += APP_VARDIR="$$VARDIR"
    DEFINES += APP_CONFFILE="$$CONFDIR/$${TARGET}.ini"

    # Install
    #
    INSTALLS += target config desktop icon32 vardir
        ## Instalar ejecutable
        target.path = $$BINDIR

        ## Instalar archivo de configuración
        config.path = $$CONFDIR
        config.files += $${TARGET}.ini

        ## Instalar acceso directo en el menú del escritorio
        desktop.path = $$DATADIR/applications
        desktop.files += $${TARGET}.desktop

        ## Instalar icono de aplicación
        icon32.path = $$DATADIR/icons/hicolor/32x32/apps
        icon32.files += ./data/32x32/$${TARGET}.png

    ## Crear directorio de archivos variables
    vardir.path = $$VARDIR
    vardir.commands = true
}


