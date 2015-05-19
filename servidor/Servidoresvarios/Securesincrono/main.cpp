#include <QCoreApplication>
#include "servidorvsincrono.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    servidorvsincrono serverSA;
    serverSA.inicioServer();
    serverSA.OpcionesLimpieza();
    return a.exec();
}
