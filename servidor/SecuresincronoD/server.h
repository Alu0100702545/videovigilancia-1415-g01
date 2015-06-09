#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include "clientet.h"

class server : public QTcpServer
{
    Q_OBJECT

public:
    explicit server(QSqlDatabase &bdd,QString RutadatosVariables,QString Rutacertificadoclave ,QObject *parent = 0);
    //~server();
protected:
    void incomingConnection(qintptr socketDescriptor);

private:
     QSqlDatabase bddc;
     QString Rutadatos, Rutacertificado;
     //QMap<qintptr,clienteT*> clients;
public slots:

};

#endif // SERVER_H
