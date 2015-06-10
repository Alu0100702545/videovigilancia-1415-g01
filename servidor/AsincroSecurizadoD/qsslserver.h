#ifndef QSSLSERVER_H
#define QSSLSERVER_H

#include <QTcpServer>
#include "client.h"
#include <QSslSocket>
class QSslServer : public QTcpServer
{

private:
    QSqlDatabase Vdb;
    QString Rutadatos, Rutacert;
public:
    QSslServer(QObject *parent , QSqlDatabase &Vdba,QString RutadatosVariables,QString Rutacertificadoclave);


public slots:
    void fallos();
protected:
    void incomingConnection(qintptr socketDescriptor) ;
};

#endif // QSSLSERVER_H
