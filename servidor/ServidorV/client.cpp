#include "client.h"

client::client(QTcpSocket *tcpSocket,QSqlDatabase &bdd, QObject *parent) : QObject(parent),
    tcpSocket_(tcpSocket),bddc(bdd)
{
     Tpaquete =0;
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
        algo=tcpSocket_->read(Tpaquete);
        //aux2=aux.toStdString(algo.toStdString());
        qDebug() << "Que me envias:"<< aux;
        paquete.ParseFromString(algo.toStdString());
        Tpaquete =0;
        almacenamiento(paquete);

    }else

    return;
}

void client::almacenamiento(VAF paquete)
{
    //QString algo(paquete.imagen().c_str());
    QByteArray buffer;
    buffer.append(paquete.imagen().c_str(),paquete.imagen().length());
    //qDebug() << buffer;
    //qDebug() << "LLEGA a almacenaminto";
    QImage im;

    qDebug() << im.loadFromData(buffer, "JPEG");
    qDebug() << im.save("joe.jpeg");


    qDebug() <<"nombre camara:"<< QString::fromStdString(paquete.nombrecamara());
    qDebug()  << "nombre pc:"<< QString::fromStdString(paquete.nombrepc());
    qDebug()  <<"Protocolo:" << QString::fromStdString(paquete.protocolo());
    qDebug()  << "timestamp:" << QString::fromStdString(paquete.timestamp());
    qDebug()  <<"timagen: " <<paquete.timagen();

    int algo=paquete.imagen().size();
   qDebug() << "imagen"<< QString::fromStdString( static_cast<std::ostringstream*>(&(std::ostringstream() << algo))->str());
    QSqlQuery query(bddc);
    query.prepare("INSERT INTO REGVAF (PRO,V,NCAMARA,NPC,TIMESTAMP) "
                  "VALUES (:PRO,:V,:NCAMARA,:NPC,:TIMESTAMP)");

    query.bindValue(":PRO",QString::fromStdString(paquete.protocolo()));
    query.bindValue(":V", QString::fromStdString(paquete.version()));
    query.bindValue(":NCAMARA", QString::fromStdString(paquete.nombrecamara()));
    query.bindValue(":NPC", QString::fromStdString(paquete.nombrepc()));
    query.bindValue(":TIMESTAMP", QString::fromStdString(paquete.timestamp()));
    //query.bindValue(":DIRECTORIO", "NULL");
    qDebug() << query.exec();
    paquete.Clear();


}


