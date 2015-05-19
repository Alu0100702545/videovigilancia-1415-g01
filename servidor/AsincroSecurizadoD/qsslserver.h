#ifndef QSSLSERVER_H
#define QSSLSERVER_H

#include <QTcpServer>
#include "client.h"
#include <QSslSocket>
class QSslServer : public QTcpServer
{

private:
    QSqlDatabase Vdb;
public:
    QSslServer(QObject *parent , QSqlDatabase &Vdba);


public slots:
    void fallos();
protected:
    void incomingConnection(qintptr socketDescriptor) ;
};

#endif // QSSLSERVER_H
