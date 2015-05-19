#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <QTcpSocket>
#include <QTcpServer>
#include "client.h"


class Servidor : public QObject
{
    Q_OBJECT
private:
    //QTcpSocket *cliente;
    QTcpServer *server;
    QSqlDatabase Vdb;
    QMap<qintptr,client*> clients;
signals:

    void algo();

public:
    void OpcionesLimpieza();
    Servidor();
    void inicioServer();
public slots:
    void conexionesPen();
    bool eliminarlista();
};

#endif // SERVIDOR_H
