#include "clientet.h"


clienteT::clienteT(qintptr socketDescriptor, QSqlDatabase &bdd,QString RutadatosVariables,QString Rutacertificadoclave,QObject *parent):
    QThread(parent),
    socketDescriptor_(socketDescriptor),
    bddc(bdd),
    contador(0)
{
    Tpaquete =0;
    Rutadata=RutadatosVariables;
    Rutacert=Rutacertificadoclave;
    paquete.Clear();
    if (!bddc.open()) {
        syslog(LOG_ERR, "NO SE PUDO ABRIR LA BDD . CERRANDO SERVIDOR");
       exit(1);
    }
}

void clienteT::run()
{
        QSslSocket tcpSocket;
        // Inicializarlo con el socket nativo de la conexión con el cliente
        if (tcpSocket.setSocketDescriptor(socketDescriptor_)) {

          connect(&tcpSocket,SIGNAL(sslErrors(QList<QSslError>)),&tcpSocket,SLOT(ignoreSslErrors()));
          tcpSocket.setProtocol(QSsl::AnyProtocol);
          tcpSocket.ignoreSslErrors();

          tcpSocket.setPrivateKey(Rutacert+"/"+"videovigilancia.key");
          tcpSocket.setPeerVerifyMode(QSslSocket::QueryPeer);
          tcpSocket.addCaCertificates(Rutacert+"/"+"videovigilancia.crt");
          tcpSocket.setLocalCertificate(Rutacert+"/"+"videovigilancia.crt");
          tcpSocket.startServerEncryption();
          tcpSocket.waitForEncrypted();
          qDebug() <<"estoy encriptado ?: "<<tcpSocket.isEncrypted();
            if (tcpSocket.isEncrypted())
                syslog(LOG_ERR, "ENCRIPTADO");
        } else {
            emit error(tcpSocket.error());
            syslog(LOG_ERR, "ERROR CONECTANDO");
            return;

        }
       do {
          if (tcpSocket.isEncrypted()){
              deserializacion(&tcpSocket);

            }else{

                return;
            }
        }while(tcpSocket.waitForReadyRead());
    //Calculo de Medias para el Benchmark
        QFile benchmark(Rutadata +
                        "/"+ QString::number(tcpSocket.socketDescriptor())+".txt");
        if(benchmark.open(QIODevice::WriteOnly)){

            QTextStream configuracion(&benchmark);
            int counter=0;
            int media=0;
            for(int i=0;i<timerrecepcionpaquetes.size()+counter && i-counter <25;i++){

                if (timerrecepcionpaquetes[i]!=0){
                        media+=timerrecepcionpaquetes[i];
                }else
                    counter++;

            }
            configuracion <<"Numero de pruebas sobre todo el proceso de paquetes capturados: "
                          << NP
                          << "\n";
            media/=NP;
            configuracion << "Media de ms sobre el proceso de paquetes capturados : "
                          <<media
                          <<"\n";

            media=0;
            for(int i=0+counter;i<timerbasedatos.size()+counter && i-counter<25;i++){
                    media+=timerbasedatos.at(i);
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

         while(tcpSocket.bytesAvailable() > 0){

             QString aux;
             QByteArray algo;
             QDataStream in(&tcpSocket);
             in.setVersion(QDataStream::Qt_4_0);
                 //Recojiendo en tamaño del paquete
              if(tcpSocket.bytesAvailable() >= (int)(sizeof(qint32))&& (Tpaquete==0))
              {
                  in >> Tpaquete;
                  aux=QString::number(Tpaquete);
                  //Teniendo el tamaño de paquete lo leemos del buffer
              } if ((Tpaquete !=0) && (tcpSocket.bytesAvailable() >=Tpaquete )){
                 algo=tcpSocket.read(Tpaquete);
                 paquete.ParseFromString(algo.toStdString());
                 Tpaquete =0;
                 almacenamiento(paquete);

             }else{

                tcpSocket.readAll();
            }
         }

        tcpSocket.disconnectFromHost();
        tcpSocket.waitForDisconnected(100);
 }

void clienteT::deserializacion(QSslSocket *tcpSocket_)
{

    timerpaquete.restart();
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

    timerrecepcionpaquetes.append(timerpaquete.elapsed());
    return;
}

void clienteT::almacenamiento(VAF &paquete)
{

    QDir directorio;
    contador++;
    QByteArray buffer;
    buffer.append(paquete.imagen().c_str(),paquete.imagen().length());
    QImage im;
    im.loadFromData(buffer, "JPEG");

   //control de paquetes
    timerbdd.restart();
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

