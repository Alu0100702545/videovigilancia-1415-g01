#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <QTcpSocket>
#include <QTcpServer>
#include "client.h"
#include "qsslserver.h"

class Servidor : public QObject
{
    Q_OBJECT
private:
    //QTcpSocket *cliente;
    QSslServer *server;
    QSqlDatabase Vdb;
    QMap<qintptr,client*> clients;
signals:

    void algo();

public:
    void OpcionesLimpieza();
    Servidor();
    void inicioServer();


};

#endif // SERVIDOR_H
