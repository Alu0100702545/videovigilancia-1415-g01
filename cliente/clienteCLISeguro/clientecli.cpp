#include "clientecli.h"

typedef std::vector<cv::Mat> ImagesType;
typedef std::vector<std::vector<cv::Point> > ContoursType;

ClienteCLI::ClienteCLI() :
    backgroundSubtractor(1000,15,false)
{
    devices = QCamera::availableDevices();
    NCamaras=devices.size();
    ListaCamaras=new QVector<CAM>;
    conexion=new QSslSocket;
    //QList<QSslError> errors;
    conexion->setProtocol(QSsl::SslV3);
    //errors.append(QSslError::SelfSignedCertificate);
    connect(conexion,SIGNAL(sslErrors(QList<QSslError>)),conexion,SLOT(ignoreSslErrors()));


    QRegExp rx("(\\,|\\/|\\:|\\t)");
    QString sometext(getenv("SESSION_MANAGER"));
    QStringList query = sometext.split(rx);
    QString NnombrePC(getenv("USER"));
    NnombrePC.append("@");
    NnombrePC.append(query.at(1));
    nombrePC=NnombrePC.toStdString();

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sigTermSd))
        qFatal("Couldn't create TERM socketpair");
    // Crear el objeto para monitorizar el segundo socket
    // de la pareja.
    sigTermNotifier = new QSocketNotifier(sigTermSd[1],
        QSocketNotifier::Read, this);
    // Conectar la señal activated() del objeto QSocketNotifier
    // con el slot handleSigTerm() para manejar la señal. Esta
    // señal será emitida cuando hayan datos para ser leidos en
    // el socket monitorizado.
    connect(sigTermNotifier, SIGNAL(activated(int)), this, SLOT(handleSigTerm()));
    /*
    //qDebug() << getenv("USERNAME");
    //qDebug() << getenv("SESSION_MANAGER");
   // qDebug() << "NombrePC: "<< NnombrePC<< " size: " << sizeof(nombrePC);;
   */
}

void ClienteCLI::termSignalHandler(int)
{
    char a = 1; // Con enviar un byte es suficiente
    write(sigTermSd[0], &a, sizeof(a));
    //qDebug() << "termSignalHanlder";
}

void ClienteCLI::handleSigTerm()
{
    // Desactivar la monitorización del socket para que por
    // el momento no lleguen más señales de Qt
    sigTermNotifier->setEnabled(false);
    // Leer y desechar el byte enviado por
    // MyDaemon::termSignalHandler()
    //qDebug() << "handleSigTerm";
    char tmp;
    read(sigTermSd[1], &tmp, sizeof(tmp));
    //MI CODIGO
    delete_all();
    // Activar la monitorización para que vuelvan a llegar
    // señales de Qt
    qApp->quit();
    sigTermNotifier->setEnabled(true);
}

ClienteCLI::~ClienteCLI()
{

}

void ClienteCLI::actualizar_cam(int indice,bool valor)
{
    QString namesetting;

    namesetting="indice";
    namesetting.append(indice+48);
    //qDebug() << "ACT NAMESETTING: " << namesetting;
    settings.setValue(namesetting,valor);
}

void ClienteCLI::actualizar_puerto(int puerto)
{
    settings.setValue("PORT",puerto);
}

void ClienteCLI::actualizar_IP(QString IP)
{
    settings.setValue("IP",IP);
}

void ClienteCLI::actualizar(){
    if(parametros==false){
        QString *IP, *camaras;

        camaras=new QString("ALL");
        IP=new QString("127.0.0.1");
        int port=33333;

        for(int i=0; i<NCamaras;i++){
            actualizar_cam(i,false);
        }

        QFile file("cliente.conf");
        if (file.open(QIODevice::ReadOnly)){
            qDebug() << "Leyendo configuracion desde fichero...";
            QTextStream configuracion(&file);

            camaras= new QString(configuracion.readLine());

            QRegExp rx("(\\,)");
            QStringList query = (*camaras).split(rx);

            if(*camaras!="ALL"){
                for(int i=0; i<query.size(); i++){
                    for(int j=0; j<NCamaras;j++){
                        if(devices[j]==query.at(i)){
                            qDebug() << "Camara " << devices[j] << " detectada";
                            actualizar_cam(j,true);
                        }
                    }
                }
            }

            IP=new QString(configuracion.readLine());
            port=(configuracion.readLine()).toInt();
        }
        else
            qDebug() << "Error al acceder al fichero cliente.conf! Cargando configuración por defecto...";

        file.close();

        if(*camaras=="ALL"){
            for(int i=0; i<NCamaras;i++){
                    actualizar_cam(i,true);
                }
        }
        actualizar_puerto(port);
        actualizar_IP(*IP);
    }
    else
        qDebug() << "Procesados parametros por terminal";
}

void ClienteCLI::capturar()
{
    CAM aux;
    QString namesetting;

    qDebug() << "IP: " << settings.value("IP").toString() << " PORT: " << settings.value("PORT").toInt();
    conexion->setPeerVerifyMode(QSslSocket::VerifyPeer);
    if (QSslSocket::supportsSsl()) {
       // connect(conexion, SIGNAL(sslErrors(QList<QSslError>)),conexion,SLOT(ignoreSslErrors()));
       //connect(socket, SIGNAL(encrypted()), this, SLOT(ready()));
      conexion->connectToHostEncrypted(settings.value("IP").toString(),settings.value("PORT").toInt());
     } else {
       qDebug()<< "No SSL Support"
         "SSL is not supported by your version of Qt. You must obtain a version of Qt"
         "that has SSL support enabled. If you believe that your version of Qt has"
         "SSL support enabled, you may need to install the OpenSSL run-time libraries.";
     }

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
            qDebug() << "Transmitiendo de " << QCamera::deviceDescription(devices[i]) << "(" << devices[i] << ")";
            connect(ListaCamaras->value(i).captureBuffer,SIGNAL(s_image(QImage,int)),this,SLOT(emitir(QImage,int)));
        }
    }
}

void ClienteCLI::emitir(const QImage &image, int id){

    cv::Mat imagenmat;
    imagenmat=QtOcv::image2Mat(image);
    cv::Mat foregroundMask;
    backgroundSubtractor(imagenmat, foregroundMask);
    cv::erode(foregroundMask, foregroundMask, cv::Mat(),cv::Point(-1,-1),10);
    cv::dilate(foregroundMask, foregroundMask, cv::Mat(),cv::Point(-1,-1),10);
    ContoursType contours;
    cv::findContours(foregroundMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

 if(conexion->isEncrypted()&&(contours.size()>0)){
    QBuffer buffer;
    QImageWriter writer;
    std::string spaquete;
    VAF paquete;

    //PROTOCOLO Y VERSION
    paquete.set_protocolo(NPROTOCOLO);
    paquete.set_version(VPROTOCOLO);

    //NOMBRE CAMARA
    std::string nombrecamara((QCamera::deviceDescription(devices[id])).toStdString());
    paquete.set_tnombrecamara(sizeof(nombrecamara));
    paquete.set_nombrecamara(nombrecamara);

    //NOMBRE PC
    paquete.set_tnombrepc(sizeof(nombrePC));
    paquete.set_nombrepc(nombrePC);

    //TIMESTAMP
    paquete.set_timestamp((QTime::currentTime().toString("hh:mm:ss:zzz")).toStdString());

    //DATESTAMP
    paquete.set_datestamp((QDate::currentDate().toString("dd.MM.yyyy")).toStdString());
    writer.setDevice(&buffer);
    writer.setFormat("jpeg");
    writer.setCompression(70);
    writer.write(image);

    //IMAGEN
    QByteArray bimagen = buffer.buffer();
    //qDebug()<< "imagen:"<< bimagen.size();
    paquete.set_timagen(bimagen.size());
    paquete.set_imagen(bimagen.data(),bimagen.size());

    //ROI
    for(int i=0; i<contours.size();i++){
        ROI* roi=paquete.add_roi();
        roi->set_x1(contours[i][1].x);
        roi->set_y1(contours[i][1].y);
        roi->set_x2(contours[i][2].x);
        roi->set_y2(contours[i][2].y);
    }

    //SERIALIZAMOS
    paquete.SerializeToString(&spaquete);

    //PAQUETE BINARIO Y TAMAÑO
    QByteArray bpaquete(spaquete.c_str(),spaquete.size());
    qint32 tbpaquete = bpaquete.size();
    //qDebug() << "TBSIZE: " << tbpaquete;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)tbpaquete;

    //ENVIO
    conexion ->write(block); //TAMAÑO
    conexion->write(bpaquete); //PAQUETE
    }
}

void ClienteCLI::delete_all(){
    QString namesetting;
    for(int i=0;i<ListaCamaras->size();i++)
    {
        namesetting="indice";
        namesetting.append(i+48);
        if((settings.value(namesetting))==true){
            ListaCamaras->value(i).Camera->stop();   
            ListaCamaras->remove(i);
            qDebug() << "Camara " << i << " eliminada!";
        }
    }
    ListaCamaras->clear();
}
