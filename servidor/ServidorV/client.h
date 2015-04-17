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
class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QTcpSocket* tcpSocket,QObject *parent = 0);
    ~client();
    qintptr getsocketDescriptor();
signals:
    void eliminar(qintptr);

public slots:
    void borrarlista();
    void deserializacion();
private:
     QTcpSocket *tcpSocket_;
     VAF paquete;
     qint32 Tpaquete;
     void almacenamiento();
};

#endif // CLIENT_H
