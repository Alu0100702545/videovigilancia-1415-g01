#ifndef CLIENT_H
#define CLIENT_H
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

class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QTcpSocket* tcpSocket,QSqlDatabase &bdd,QObject *parent = 0);
    ~client();
    QTcpSocket *get_tcp();
    qintptr getsocketDescriptor();

signals:
    void eliminar(qintptr);

public slots:
    void borrarlista();
    void deserializacion();
    void limpiarbuffer();
private:
     QTcpSocket *tcpSocket_;
     qint32 Tpaquete;
     QSqlDatabase bddc;
     VAF paquete;
     void almacenamiento(VAF);
     qintptr socket;
};

#endif // CLIENT_H
