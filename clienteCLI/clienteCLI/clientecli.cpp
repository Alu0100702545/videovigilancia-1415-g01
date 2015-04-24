#include "clientecli.h"

ClienteCLI::ClienteCLI()
{
    devices = QCamera::availableDevices();
    NCamaras=devices.size();
    ListaCamaras=new QVector<CAM>;
    conexion=new QTcpSocket;

    QRegExp rx("(\\,|\\/|\\:|\\t)");
    QString sometext(getenv("SESSION_MANAGER"));
    QStringList query = sometext.split(rx);
    QString NnombrePC(getenv("USER"));
    NnombrePC.append("@");
    NnombrePC.append(query.at(1));
    nombrePC=NnombrePC.toStdString();
    /*
    //qDebug() << getenv("USERNAME");
    //qDebug() << getenv("SESSION_MANAGER");
   // qDebug() << "NombrePC: "<< NnombrePC<< " size: " << sizeof(nombrePC);;
   */
}

ClienteCLI::~ClienteCLI()
{

}

void ClienteCLI::debug(){
    qDebug() << "HOLA";
}

void ClienteCLI::actualizar_cam(int indice,bool valor)
{
    QString namesetting;

    namesetting="indice";
    namesetting.append(indice);
    //qDebug() << "ACT NAMESETTING: " << namesetting;
    settings.setValue(namesetting,valor);
}

void ClienteCLI::actualizar_puerto(int puerto)
{
    settings.setValue("puerto",puerto);
}

void ClienteCLI::actualizar_IP(QString IP)
{
    settings.setValue("IP",IP);
}

void ClienteCLI::actualizar(){
    /*
     * LECTURA DESDE FICHERO DE LAS CAMARAS, IP Y PUERTO
     */
    QVector<int> activas;
    int indice=0,port;
    QString IP;
    bool todas=false;

    //LEER DE FICHERO LAS CAMARAS
    //PUSH DE LOS INDICES, SI HAY UN -1 SON TODAS ACTIVAS

    for(int i=0; i<NCamaras;i++){
        if(todas)
            actualizar_cam(i,true);
        else if(i==activas.at(indice)){
            actualizar_cam(i,true);
            indice++;
        }
        else{
            actualizar_cam(i,false);

        }
    }

    actualizar_puerto(port);
    actualizar_IP(IP);
}

void ClienteCLI::capturar()
{
    CAM aux;
    QString namesetting;

    conexion->connectToHost(settings.value("IP").toString(),settings.value("PORT").toInt());

    for(int i=0;i<NCamaras;i++)
    {
        namesetting="indice";
        namesetting.append(i+48);
        //qDebug() << "CAPT NAMESETTING: " << namesetting;
        if((settings.value(namesetting))==true){
            aux.Camera=new QCamera(devices[i]);
            aux.captureBuffer=new CaptureBuffer;
            aux.captureBuffer->id=i;
            ListaCamaras->push_back(aux);
            ListaCamaras->value(i).Camera->setViewfinder(ListaCamaras->value(i).captureBuffer);
            ListaCamaras->value(i).Camera->start();
            connect(ListaCamaras->value(i).captureBuffer,SIGNAL(s_image(QImage,int)),this,SLOT(emitir(QImage,int)));
        }
    }
}

void ClienteCLI::emitir(const QImage &image, int id){

    //required string protocolo = 1;
    //required bytes  version = 2;
    //optional uint32  Tnombrecamara = 3 ;
    //required string  nombrecamara = 4;
    //optional uint32  TnombrePC = 5 ;
    //required string  nombrePC = 6;
    //required string  timestamp = 7;
    //optional uint32  TImagen=8;
    //required string  imagen=9;

    QBuffer buffer;
    QImageWriter writer;
    std::string spaquete;
    VAF paquete;

    paquete.set_protocolo(NPROTOCOLO);
    paquete.set_version(VPROTOCOLO);

    //QString dprotocolo(paquete.protocolo().c_str());
    //QString diversion(paquete.version().c_str());
    //qDebug() << dprotocolo << diversion;

    std::string nombrecamara((QCamera::deviceDescription(devices[id])).toStdString());
    paquete.set_tnombrecamara(sizeof(nombrecamara));
    paquete.set_nombrecamara(nombrecamara);

    //qint32 dtnombrecamara(paquete.tnombrecamara());
    //QString dnombrecamara(paquete.nombrecamara().c_str());
    //qDebug() << dnombrecamara << dtnombrecamara;

    paquete.set_tnombrepc(sizeof(nombrePC));
    paquete.set_nombrepc(nombrePC);

    //qint32 dtnombrepc(paquete.tnombrepc());
    //QString dnombrepc(paquete.nombrepc().c_str());
    //qDebug() << dnombrepc << dtnombrepc;

    paquete.set_timestamp((QTime::currentTime().toString("hh:mm:ss:zzz")).toStdString());

    //QString dtime(paquete.timestamp().c_str());
    //qDebug() << dtime;

    writer.setDevice(&buffer);
    writer.setFormat("jpeg");
    writer.setCompression(70);
    writer.write(image);
    QByteArray bimagen = buffer.buffer();
    //qDebug()<< "imagen:"<< bimagen.size();
    //QString imagen(bimagen);
    paquete.set_timagen(bimagen.size());
    paquete.set_imagen(bimagen.data(),bimagen.size());

    //qint32 dtimagen((paquete.timagen()));
    //QString dimagen(paquete.imagen().c_str());
    //qDebug() << dimagen << dtimagen;

    paquete.SerializeToString(&spaquete);

    QByteArray bpaquete(spaquete.c_str(),spaquete.size());
    qint32 tbpaquete = bpaquete.size();
    //qDebug() << "TBSIZE: " << tbpaquete;
    //QByteArray btbpaquete;
    //btbpaquete.append((const char*)&tbpaquete,sizeof(qint32));
    //btbpaquete.append('\n');

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)tbpaquete;
    out << tbpaquete;

    conexion ->write(block);
    //qDebug() << "sizeof mandado OK";
    conexion->write(bpaquete);
    //qDebug() << "bpaquete mandado OK";

}
