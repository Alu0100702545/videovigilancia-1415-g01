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
    Servidor();
    void inicioServer();
public slots:
    void conexionesPen();
    void eliminarlista(qintptr);
};

#endif // SERVIDOR_H
