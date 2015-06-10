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
#include <QTimer>
#include <QTime>
#define NP 30
class clienteT : public QThread
{

    Q_OBJECT
public:
    clienteT(qintptr socketDescriptor, QSqlDatabase &bdd,QString RutadatosVariables,QString Rutacertificadoclave,QObject *parent);

    void run();
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
    QVector<int> timerbasedatos;
    QVector<int> timerrecepcionpaquetes;
    QTime timerpaquete;
    QTime timerbdd;
};

#endif // CLIENTET_H
