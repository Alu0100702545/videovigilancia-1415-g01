#include "client.h"

client::client(QTcpSocket *tcpSocket, QObject *parent) : QObject(parent),
    tcpSocket_(tcpSocket)
{
     Tpaquete =0;
    connect(tcpSocket_, SIGNAL(readyRead()), this, SLOT(deserializacion()));
    connect(tcpSocket_,SIGNAL(disconnected()), this, SLOT(borrarlista()));
}
client::~client()
{

}

qintptr client::getsocketDescriptor()
{
    return tcpSocket_->socketDescriptor();
}

void client::borrarlista()
{
    emit eliminar(tcpSocket_->socketDescriptor());
}
void client::deserializacion()
{

    QString aux;
    std::string aux2;
    QByteArray algo, algo2;
    qint32 tint=sizeof(qint32);
        qDebug() << tcpSocket_->bytesAvailable();
    if (tcpSocket_->bytesAvailable() >= sizeof(qint32) && (Tpaquete==0)){
        algo=tcpSocket_->read(tint);
        qDebug() <<QString ::fromLatin1(algo);
        Tpaquete=algo.toInt();
        aux=QString::number(Tpaquete);
        qDebug() << aux;
     }else if ((Tpaquete !=0) && (tcpSocket_->bytesAvailable() >=Tpaquete )){


        aux=tcpSocket_->read(Tpaquete);
        aux2=aux.toStdString();
        qDebug() << aux;
        paquete.ParseFromString(aux2);
        Tpaquete =0;
        almacenamiento();

    }else

    return;
}

void client::almacenamiento()
{
    QString algo =QString::fromStdString(paquete.imagen());
    QByteArray buffer(algo.toLocal8Bit());
    //qDebug() << buffer;
    qDebug() << "LLEGA a almacenaminto";
    QImage im;
    im.loadFromData(buffer, "JPEG");
    im.save("/tmp");
    qDebug() << QString::fromStdString(paquete.nombrecamara());
    qDebug()  << QString::fromStdString(paquete.nombrepc());
    qDebug()  << QString::fromStdString(paquete.protocolo());
    qDebug()  << QString::fromStdString(paquete.timestamp());
    qDebug()  << paquete.timagen();
}


