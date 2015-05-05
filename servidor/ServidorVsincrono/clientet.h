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
class clienteT : public QThread
{

    Q_OBJECT
public:
    clienteT(qintptr socketDescriptor, QSqlDatabase &bdd,QObject *parent);
        //: QThread(parent), socketDescriptor_(socketDescriptor){}
    void run();
    //~clienteT();
    void deserializacion(QTcpSocket * tcpSocket_);
    void almacenamiento(VAF &paquete);
signals:
    void error(QTcpSocket::SocketError socketError);
    void eliminacion();

private:
    qintptr socketDescriptor_;
    qint32 Tpaquete;
    QSqlDatabase bddc;
    VAF paquete;
    qint64 contador;
};

#endif // CLIENTET_H
