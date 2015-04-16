#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <QTcpSocket>
#include <QTcpSocket>
#include "VAF.pb.h"
#include <QTcpServer>
#include <QString>
#include <QByteArray>
#include <string>
#include <QObject>


class Servidor : public QObject
{
    Q_OBJECT
private:
    QTcpSocket *client;
    QTcpServer *server;
    VAF paquete;
    qint32 Tpaquete;
signals:

    void algo();
public slots:

    void deserializacion();
public:
    Servidor();
    void almacenamiento();
};

#endif // SERVIDOR_H
