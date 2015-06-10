#include "client.h"

client::client(QSslSocket *tcpSocket,QSqlDatabase &bdd, QString RutadatosVariables,QObject *parent) : QObject(parent),
    tcpSocket_(tcpSocket),bddc(bdd)
{
    Rutadata=RutadatosVariables;
    Tpaquete =0;
    connect(tcpSocket_, SIGNAL(readyRead()), this, SLOT(deserializacion()));
    connect(tcpSocket_,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(fallos()));
    connect(tcpSocket_,SIGNAL(sslErrors(QList<QSslError>)),this,SLOT(fallos()));
    connect(tcpSocket_,SIGNAL(disconnected()), this, SLOT(limpiarbuffer()));

    if (!bddc.open()) {
        syslog(LOG_ERR, "NO SE PUDO ABRIR LA BDD . CERRANDO SERVIDOR");
        exit(1);
    }

}
client::~client()
{

}

QSslSocket* client::get_tcp()
{
    return tcpSocket_;
}

qintptr client::getsocketDescriptor()
{
    return tcpSocket_->socketDescriptor();
}

void client::limpiarbuffer()
{
     while(tcpSocket_->bytesAvailable() > 0){
         QByteArray algo;
         QDataStream in(tcpSocket_);
         in.setVersion(QDataStream::Qt_4_0);
             //Recojiendo en tamaño del paquete
          if(tcpSocket_->bytesAvailable() >= (int)(sizeof(qint32))&& (Tpaquete==0))
          {
              in >> Tpaquete;
              //Teniendo el tamaño de paquete lo leemos del buffer
          } if ((Tpaquete !=0) && (tcpSocket_->bytesAvailable() >=Tpaquete )){
             algo=tcpSocket_->read(Tpaquete);
             paquete.ParseFromString(algo.toStdString());
             Tpaquete =0;
             almacenamiento(paquete);

         }else
            tcpSocket_->readAll();

     }
      tcpSocket_->disconnectFromHost();
     QFile benchmark(Rutadata +
                     "/"+ QString::number(tcpSocket_->socketDescriptor())+".txt");
     if(benchmark.open(QIODevice::WriteOnly)){
        QTextStream configuracion(&benchmark);

        int media=0;
        for(int i=0;i<timerrecepcionpaquetes.size() && i<NP;i++){
                 media+=timerrecepcionpaquetes[i];

        }

        configuracion <<"Numero de pruebas sobre todo el proceso de paquetes capturados: "
                      << NP
                      << "\n";
        media/=NP;
        configuracion << "Media de ms sobre el proceso de paquetes capturados : "
                      <<media
                      <<"\n";

        media=0;
        for(int i=0;i<timerbasedatos.size() && i <NP;i++){
            media+=timerbasedatos[i];
        }
        configuracion <<" Numero de pruebas en la bdd "
                        << NP
                        << "\n";


        media/=NP;
        configuracion << "Media ms en la bdd: "
                        <<media
                        <<"\n";
        }
     benchmark.close();

}

void client::borrarlista()
{
    emit eliminar(tcpSocket_->socketDescriptor());
}
void client::deserializacion()
{
    QTime timerpaquete;
    timerpaquete.start();
    QByteArray algo;
    QDataStream in(tcpSocket_);
    in.setVersion(QDataStream::Qt_4_0);
        //Recojiendo en tamaño del paquete

     if(tcpSocket_->bytesAvailable() >= (int)(sizeof(qint32))&& (Tpaquete==0))
     {
         in >> Tpaquete;
         //Teniendo el tamaño de paquete lo leemos del buffer
     }
     if ((Tpaquete !=0) && (tcpSocket_->bytesAvailable() >=Tpaquete )){
        algo=tcpSocket_->read(Tpaquete);
        paquete.ParseFromString(algo.toStdString());
        Tpaquete =0;
        almacenamiento(paquete);
    }
    ///timerpaquete.stop();
  if ( timerpaquete.elapsed()>1)
        timerrecepcionpaquetes.append(timerpaquete.elapsed());
    return;
}

void client::almacenamiento(VAF paquete)
{
    QDir directorio;
    //contador++;
    QByteArray buffer;
    buffer.append(paquete.imagen().c_str(),paquete.imagen().length());
    QImage im;
    im.loadFromData(buffer, "JPEG");

   //control de paquetes
    QTime timerbdd;
    timerbdd.start();
    //introducciendo en la Base de Datos
    QSqlQuery query(bddc);
    query.prepare("INSERT INTO REGVAF (PRO,V,NCAMARA,NPC,DATESTAMP,TIMESTAMP,DIRECTORIO) "
                  "VALUES (:PRO,:V,:NCAMARA,:NPC,:DATESTAMP,:TIMESTAMP,:DIRECTORIO)");

    query.bindValue(":PRO",QString::fromStdString(paquete.protocolo()));
    query.bindValue(":V", QString::fromStdString(paquete.version()));
    query.bindValue(":NCAMARA", QString::fromStdString(paquete.nombrecamara()));
    query.bindValue(":NPC", QString::fromStdString(paquete.nombrepc()));
    query.bindValue(":TIMESTAMP", QString::fromStdString(paquete.timestamp()));
    query.bindValue(":DATESTAMP", QString::fromStdString(paquete.datestamp()));

    //Hayando la ruta de la foto
    QString pc=QString::fromStdString(
                QString::fromStdString(paquete.nombrepc()).toUtf8().toHex().toStdString());

    QString camara=QString::fromStdString(
                QString::fromStdString(paquete.nombrecamara()).toUtf8().toHex().toStdString());

    QString date=QString::fromStdString(
                QString::fromStdString(paquete.datestamp()).toUtf8().toHex().toStdString());

    std::string time= QString::fromStdString(paquete.timestamp()).toUtf8().toHex().toStdString();


    QString hora= QString::fromStdString(
                time.substr(0,6));

    QString minutos= QString::fromStdString(
                time.substr(6,6));

    QString segundos=QString::fromStdString(
                time.substr(12,6));
    QString segundosMs=QString::fromStdString(
                time.substr(18,time.size()-18));


    QString direct=  Rutadata +
            "/"+"clientes/" +pc+"/"+camara+"/"+date+"/"+hora+
            "/"+ minutos+"/"+segundos+"/" +segundosMs +".jpeg";
    query.bindValue(":DIRECTORIO",direct);

    directorio.mkpath( Rutadata +"/"+"clientes/" +pc+"/"+camara+"/"+date+"/"+hora+
                      "/"+ minutos+"/"+segundos);

    query.exec();



    im.save(direct);
    //Limpieza del paquete
    paquete.roi_size() ;
    for(int i=0;i <paquete.roi_size();i++){
        query.prepare("INSERT INTO ROI (DIRECTORIO,CX1,CX2,CY1,CY2) "
                      "VALUES (:DIRECTORIO,:CX1,:CX2,:CY1,:CY2)");
        query.bindValue(":DIRECTORIO",direct);
        query.bindValue(":CX1",paquete.roi(i).x1());
        query.bindValue(":CX2",paquete.roi(i).x2());
        query.bindValue(":CY1",paquete.roi(i).y1());
        query.bindValue(":CY2",paquete.roi(i).y2());
        query.exec() ;
    }

    timerbasedatos.append(timerbdd.elapsed());
 paquete.Clear();
}


void client::fallos()
{
    syslog(LOG_ERR,"FALLO DESCONOCIDO");
}
