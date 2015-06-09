#ifndef CLIENTET_H
#define CLIENTET_H
#include "VAF.pb.h"
#include <QThread>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTcpSocket>
#include <QDir>
#include <QImage>
#include <QBuffer>
#include <QDirIterator>
#include <iostream>
#include <QSslSocket>
#include <syslog.h>
#include <unistd.h>
class clienteT : public QThread
{

    Q_OBJECT
public:
    clienteT(qintptr socketDescriptor, QSqlDatabase &bdd,QString RutadatosVariables,QString Rutacertificadoclave,QObject *parent);
        //: QThread(parent), socketDescriptor_(socketDescriptor){}
    void run();
    //~clienteT();
    void deserializacion(QSslSocket * tcpSocket_);
    void almacenamiento(VAF &paquete);
signals:
    void error(QSslSocket::SocketError socketError);
    void eliminacion();

private:
    qintptr socketDescriptor_;
    qint32 Tpaquete;
    QSqlDatabase bddc;
    VAF paquete;
    qint64 contador;
    QString Rutadata, Rutacert;


};

#endif // CLIENTET_H
