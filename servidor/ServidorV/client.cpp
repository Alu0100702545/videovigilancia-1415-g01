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

    QString aux, aux3;
    std::string aux2;
    QByteArray algo;
        //qDebug() << tcpSocket_->bytesAvailable();
     if(tcpSocket_->canReadLine()&& (Tpaquete==0))
     {
         algo = tcpSocket_->readLine();
         aux3 = QString ::fromLocal8Bit(algo);
         aux3.remove('\n');
         qDebug() <<aux3;
         Tpaquete=aux3.toInt();
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


