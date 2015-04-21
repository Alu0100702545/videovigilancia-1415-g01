#include <QCoreApplication>
#include "servidor.h"
#include <QRegExp>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Servidor server;
    server.inicioServer();
    /*QRegExp rx("(\\,|\\/|\\:|\\t)");
    QString sometext(getenv("SESSION_MANAGER"));
    QStringList query = sometext.split(rx);
    QString nombrePC(getenv("USER"));
    nombrePC.append("@");
    nombrePC.append(query.at(1));
    qDebug()<< nombrePC;
    qDebug() << query.at(1);
    qDebug() << getenv("USERNAME");
    qDebug() << getenv("USER");*/

    return a.exec();
}
