#ifndef CLIENT_H
#define CLIENT_H

#include <syslog.h>

#include "VAF.pb.h"
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QString>
#include <QByteArray>
#include <string>
#include <QObject>
#include <QMap>
#include <QImage>
#include <QDebug>
#include <sstream>
#include <iostream>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <sqlite3.h>
#include <sqldb.h>
#include <QSqlDriver>
#include <QDir>
#include <iostream>
#include <QSslSocket>
#include <openssl/ssl.h>
#include <QTime>

#define NP 30
class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QSslSocket* tcpSocket,QSqlDatabase &bdd,QString RutadatosVariables,QObject *parent = 0);
    ~client();
    QSslSocket *get_tcp();
    qintptr getsocketDescriptor();

signals:
    void eliminar(qintptr);

public slots:
    void borrarlista();
    void deserializacion();
    void limpiarbuffer();

    void fallos();
private:
     QSslSocket *tcpSocket_;
     qint32 Tpaquete;
     QSqlDatabase bddc;
     QString Rutadata;
     VAF paquete;
     void almacenamiento(VAF);
     qintptr socket;
     QVector<int> timerbasedatos;
     QVector<int> timerrecepcionpaquetes;


};

#endif // CLIENT_H
