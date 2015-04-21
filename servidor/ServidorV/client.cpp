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
    QDataStream in(tcpSocket_);
    in.setVersion(QDataStream::Qt_4_0);
        //qDebug() << tcpSocket_->bytesAvailable();
     if(tcpSocket_->bytesAvailable() >= (int)(sizeof(qint32))&& (Tpaquete==0))
     {
         //algo = tcpSocket_->read(sizeof(Tpaquete));
         //aux3 = QString ::fromLocal8Bit(algo);
         //aux3.remove('\n');
         //qDebug() <<aux3;
         //Tpaquete=aux3.toInt();
         in >> Tpaquete;
         //qDebug() << QString::fromStdString(algo.toStdString());
        // Tpaquete =(qint32)algo.toStdString();
         aux=QString::number(Tpaquete);
         qDebug() <<"tamaño paquete:"<< aux;

     }else if ((Tpaquete !=0) && (tcpSocket_->bytesAvailable() >=Tpaquete )){
        aux=tcpSocket_->read(Tpaquete);
        aux2=aux.toStdString();
        qDebug() << "Que me envias:"<< aux;
        paquete.ParseFromString(aux2);
        Tpaquete =0;
        almacenamiento();

    }else

    return;
}

void client::almacenamiento()
{
    //QString algo =QString::fromStdString(paquete.imagen());
    //QByteArray buffer(algo.toUInt());
    //qDebug() << buffer;
    //qDebug() << "LLEGA a almacenaminto";
    //QImage im;
    //qDebug() << im.loadFromData(buffer, "JPEG");
    //qDebug() << im.save("joe.jpeg");


    qDebug() <<"nombre camara:"<< QString::fromStdString(paquete.nombrecamara());
    qDebug()  << "nombre pc:"<< QString::fromStdString(paquete.nombrepc());
    qDebug()  <<"Protocolo:" << QString::fromStdString(paquete.protocolo());
    qDebug()  << "timestamp:" << QString::fromStdString(paquete.timestamp());
    qDebug()  <<"timagen: " <<paquete.timagen();

    int algo=paquete.imagen().size();
   // if (paquete.imagen().size()== paquete.timagen())


        qDebug() << "imagen"<< QString::fromStdString( static_cast<std::ostringstream*>(&(std::ostringstream() << algo))->str());
}


