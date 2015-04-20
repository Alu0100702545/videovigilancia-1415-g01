#include "client.h"

client::client(QTcpSocket *tcpSocket,QSqlDatabase &bdd, QObject *parent) : QObject(parent),
    tcpSocket_(tcpSocket),bddc(bdd)
{
    connect(tcpSocket_, SIGNAL(readyRead()), this, SLOT(deserializacion()));
    connect(tcpSocket_,SIGNAL(disconnected()), this, SLOT(borrarlista()));


    if (!bddc.open()) {
        qDebug() <<"No se pudo acceder a los datos";
        exit(1);
    }

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
    VAF paquete;
    QString aux;
    std::string aux2;
    if ((unsigned(tcpSocket_->bytesAvailable()) >= sizeof(qint32)) && (Tpaquete==0))
        Tpaquete=tcpSocket_->read(sizeof(qint32)).toInt();
    if (Tpaquete !=0 && tcpSocket_->bytesAvailable() >=Tpaquete ){
        aux=tcpSocket_->read(Tpaquete);
        aux2=aux.toStdString();
        paquete.ParseFromString(aux2);
        Tpaquete =0;
        almacenamiento(paquete);

    }
    return;
}

void client::almacenamiento(VAF paquete)
{
    QString algo =QString::fromStdString(paquete.imagen());
    QByteArray buffer(algo.toLocal8Bit());
    qDebug() << buffer;

    QImage im;
    im.loadFromData(buffer, "JPEG");
    im.save("/tmp");
    QSqlQuery query(bddc);
    query.prepare("INSERT INTO REGVAF (PRO,V,NCAMARA,NPC,TIMESTAMP,DIRECTORIO) "
                  "VALUES (:PRO,:V,:NCAMARA,:NPC,:TIMESTAMP,:DIRECTORIO)");

    query.bindValue(":PRO",QString::fromStdString(paquete.protocolo()));
    query.bindValue(":V", QString::fromStdString(paquete.version()).toLocal8Bit());
    query.bindValue(":NCAMARA", QString::fromStdString(paquete.nombrecamara()));
    query.bindValue(":NPC", QString::fromStdString(paquete.nombrepc()));
    query.bindValue(":TIMESTAMP", QString::fromStdString(paquete.timestamp()));
    query.bindValue(":DIRECTORIO", "NULL");
    query.exec();



    qDebug() << QString::fromStdString(paquete.nombrecamara());
    qDebug()  << QString::fromStdString(paquete.nombrepc());
    qDebug()  << QString::fromStdString(paquete.protocolo());
    qDebug()  << QString::fromStdString(paquete.timestamp());
    qDebug()  << paquete.timagen();
}


