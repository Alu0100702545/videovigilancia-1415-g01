#include <QCoreApplication>
#include "servidor.h"
#include <QRegExp>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Servidor server;
    server.inicioServer();
    server.OpcionesLimpieza();

    return a.exec();
}
