#include "clientev.h"
#include "ui_clientev.h"

typedef std::vector<cv::Mat> ImagesType;
typedef std::vector<std::vector<cv::Point> > ContoursType;

ClienteV::ClienteV(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClienteV),
    backgroundSubtractor(1000,15,false)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->setupUi(this);

    if(settings.value("transmitir")==true)
        ui->CheckTransmitir->setChecked(true);

    devices = QCamera::availableDevices();
    NCamaras=devices.size();
    ListaCamaras=new QVector<CAM>;
    conexion=new QSslSocket;
    QList<QSslError> errors;
    conexion->setProtocol(QSsl::SslV3);
    errors.append(QSslError::SelfSignedCertificate);
    connect(conexion,SIGNAL(sslErrors(QList<QSslError>)),conexion,SLOT(ignoreSslErrors()));
     connect(conexion,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error(QAbstractSocket::SocketError)));


     //qDebug() << getenv("USERNAME");
    //qDebug() << getenv("SESSION_MANAGER");

    QRegExp rx("(\\,|\\/|\\:|\\t)");
    QString sometext(getenv("SESSION_MANAGER"));
    QStringList query = sometext.split(rx);
    QString NnombrePC(getenv("USER"));
    NnombrePC.append("@");
    NnombrePC.append(query.at(1));
    nombrePC=NnombrePC.toStdString();
   // qDebug() << "NombrePC: "<< NnombrePC<< " size: " << sizeof(nombrePC);;


}

ClienteV::~ClienteV()
{
    delete ui;
}

void ClienteV::on_BotonCerrar_clicked()
{
    conexion->disconnectFromHost();
    qApp->quit();
}

void ClienteV::on_actionCerrar_triggered()
{
    conexion->disconnectFromHost();
    qApp->quit();
}

void ClienteV::on_CheckTransmitir_clicked()
{
    if(settings.value("transmitir")==false)
        settings.setValue("transmitir" ,true);
    else
        settings.setValue("transmitir" ,false);
}

void ClienteV::on_actionProyecto_Video_triggered()
{
    AcercaDe *about;
    about= new AcercaDe;
    about->show();
}

void ClienteV::on_actionOpciones_triggered()
{
    combobox* preferencias=new combobox(devices);

    preferencias->show();
    connect(preferencias,SIGNAL(s_camera(int,bool)),this,SLOT(actualizar_cam(int,bool)));
    connect(preferencias,SIGNAL(s_port(int)),this,SLOT(actualizar_puerto(int)));
    connect(preferencias,SIGNAL(s_IP(QString)),this,SLOT(actualizar_IP(QString)));
}

void ClienteV::actualizar_cam(int indice,bool valor)
{
    QString namesetting;

    namesetting="indice";
    namesetting.append(indice);
    //qDebug() << "ACT NAMESETTING: " << namesetting;
    settings.setValue(namesetting,valor);
}

void ClienteV::actualizar_puerto(int puerto)
{
    settings.setValue("puerto",puerto);
}

void ClienteV::actualizar_IP(QString IP)
{
    settings.setValue("IP",IP);
}

void ClienteV::on_BotonCapturar_clicked()
{
    CAM aux;
    QString namesetting;
    int pos=0;
    int NLabelsX=0, NLabelsY=0;

    if(settings.value("transmitir")==true){

        conexion->setPeerVerifyMode(QSslSocket::VerifyPeer);
        if (QSslSocket::supportsSsl()) {
           // connect(conexion, SIGNAL(sslErrors(QList<QSslError>)),conexion,SLOT(ignoreSslErrors()));
           //connect(socket, SIGNAL(encrypted()), this, SLOT(ready()));
          conexion->connectToHostEncrypted(settings.value("IP").toString(),settings.value("PORT").toInt());
         } else {
           QMessageBox::critical(this, "No SSL Support",
             "SSL is not supported by your version of Qt. You must obtain a version of Qt"
             "that has SSL support enabled. If you believe that your version of Qt has"
             "SSL support enabled, you may need to install the OpenSSL run-time libraries.");
         }
    }
    //qDebug() << NCamaras;
    //qDebug() << "conexion" <<conexion->errorString();
    if(NCamaras==2){
        NLabelsY=1;
        NLabelsX=2;
    }
    else if(NCamaras%2==0){
        NLabelsX=NCamaras/2;
        NLabelsY=NCamaras/2;
    }
    else{
        NLabelsX=(NCamaras+1)/2;
        NLabelsY=(NCamaras+1)/2;
    }

    for(int i=0;i<NLabelsY;i++){
        for(int j=0;j<NLabelsX;j++){
            QLabel* label=new QLabel;
            label->setScaledContents(true);
            ui->gridLayout->addWidget(label,i,j);
            //qDebug() << "AÑADIDO EN " << i << j;
        }
    }

    for(int i=0;i<NCamaras;i++)
    {
        namesetting="indice";
        namesetting.append(i+48);
        //qDebug() << "CAPT NAMESETTING: " << namesetting;
        if((settings.value(namesetting))==true)
        {
            //qDebug() << "ENTRAMOS CON " << namesetting;
            aux.Camera=new QCamera(devices[i]);
            aux.captureBuffer=new CaptureBuffer;
            aux.captureBuffer->pos=pos;
            aux.id=i;
            ListaCamaras->push_back(aux);
            ListaCamaras->value(pos).Camera->setViewfinder(ListaCamaras->value(pos).captureBuffer);
            ListaCamaras->value(pos).Camera->start();
            connect(ListaCamaras->value(pos).captureBuffer,SIGNAL(s_image(QImage,int)),this,SLOT(image_s(QImage,int)));
            pos++;
        }
    }
}

void ClienteV::on_actionCapturar_triggered()
{
    CAM aux;
    QString namesetting;
    int pos=0;
    int NLabelsX=0, NLabelsY=0;

    if(settings.value("transmitir")==true){

        conexion->setPeerVerifyMode(QSslSocket::VerifyPeer);
        if (QSslSocket::supportsSsl()) {

           //connect(socket, SIGNAL(encrypted()), this, SLOT(ready()));

            connect(conexion, SIGNAL(sslErrors(QList<QSslError>)),conexion,SLOT(ignoreSslErrors()));
            conexion->connectToHostEncrypted(settings.value("IP").toString(),settings.value("PORT").toInt());
         } else {
           QMessageBox::critical(this, "No SSL Support",
             "SSL is not supported by your version of Qt. You must obtain a version of Qt"
             "that has SSL support enabled. If you believe that your version of Qt has"
             "SSL support enabled, you may need to install the OpenSSL run-time libraries.");
         }
    }
    if(NCamaras==2){
        NLabelsY=1;
        NLabelsX=2;
    }
    else if(NCamaras%2==0){
        NLabelsX=NCamaras/2;
        NLabelsY=NCamaras/2;
    }
    else{
        NLabelsX=(NCamaras+1)/2;
        NLabelsY=(NCamaras+1)/2;
    }

    for(int i=0;i<NLabelsY;i++){
        for(int j=0;j<NLabelsX;j++){
            QLabel* label=new QLabel;
            label->setScaledContents(true);
            ui->gridLayout->addWidget(label,i,j);
            //qDebug() << "AÑADIDO EN " << i << j;
        }
    }

    for(int i=0;i<NCamaras;i++)
    {
        namesetting="indice";
        namesetting.append(i+48);
        //qDebug() << "CAPT NAMESETTING: " << namesetting;
        if((settings.value(namesetting))==true)
        {
            //qDebug() << "ENTRAMOS CON " << namesetting;
            aux.Camera=new QCamera(devices[i]);
            aux.captureBuffer=new CaptureBuffer;
            aux.captureBuffer->pos=pos;
            aux.id=i;
            ListaCamaras->push_back(aux);
            ListaCamaras->value(pos).Camera->setViewfinder(ListaCamaras->value(pos).captureBuffer);
            ListaCamaras->value(pos).Camera->start();
            connect(ListaCamaras->value(pos).captureBuffer,SIGNAL(s_image(QImage,int)),this,SLOT(image_s(QImage,int)));
            pos++;
        }
    }
}

void ClienteV::emitir(const QImage &image, const int &pos, ContoursType contours){

    QBuffer buffer;
    QImageWriter writer;
    std::string spaquete;
    VAF paquete;

    //PROTOCOLO Y VERSION
    paquete.set_protocolo(NPROTOCOLO);
    paquete.set_version(VPROTOCOLO);

    //NOMBRE CAMARA
    std::string nombrecamara((QCamera::deviceDescription(devices[ListaCamaras->value(pos).id])).toStdString());
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
    qDebug()<< "imagen:"<< bimagen.size();
    paquete.set_timagen(bimagen.size());
    paquete.set_imagen(bimagen.data(),bimagen.size());

    //ROI
    for(int i=0; i<contours.size();i++){
        ROI* roi=paquete.add_roi();
        roi->set_x1=contours[i][1].x;
        roi->set_y1=contours[i][1].y;
        roi->set_x2=contours[i][2].x;
        roi->set_y2=contours[i][2].y;
    }

    //SERIALIZAMOS
    paquete.SerializeToString(&spaquete);

    //PAQUETE BINARIO Y TAMAÑO
    QByteArray bpaquete(spaquete.c_str(),spaquete.size());
    qint32 tbpaquete = bpaquete.size();
    qDebug() << "TBSIZE: " << tbpaquete;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)tbpaquete;

    //ENVIO
    conexion ->write(block); //TAMAÑO
    conexion->write(bpaquete); //PAQUETE

}

void ClienteV::image_s(const QImage &image, const int &pos)
{

    cv::Mat imagen;
    imagen=QtOcv::image2Mat(image);
    //backgroundSubtractor.nmixtures = 3;
    //for (ImagesTypes::const_iterator i = images.begin(); i < images.end(); ++i) {
        // Sustracción del fondo:
        //  1. El objeto sustractor compara la imagen en i con su
        //     estimación del fondo y devuelve en foregroundMask una
        //     máscara (imagen binaria) con un 1 en los píxeles de
        //     primer plano.
        //  2. El objeto sustractor actualiza su estimación del fondo
        //     usando la imagen en i.
        cv::Mat foregroundMask;
        backgroundSubtractor(imagen, foregroundMask);
        // Operaciones morfolóficas para eliminar las regiones de
        // pequeño tamaño. Erode() las encoge y dilate() las vuelve a
        // agrandar.
        cv::erode(foregroundMask, foregroundMask, cv::Mat(),cv::Point(-1,-1),10);
        cv::dilate(foregroundMask, foregroundMask, cv::Mat(),cv::Point(-1,-1),10);
        // Obtener los contornos que bordean las regiones externas
        // (CV_RETR_EXTERNAL) encontradas. Cada contorno es un vector
        // de puntos y se devuelve uno por región en la máscara.
        ContoursType contours;
        cv::findContours(foregroundMask, contours, CV_RETR_EXTERNAL,
                         CV_CHAIN_APPROX_NONE);
        // Aquí va el código ódigo que usa los contornos encontrados...
        // P. ej. usar cv::boundingRect() para obtener el cuadro
        // delimitador de cada uno y pintarlo en la imagen original
        for(int i=0; i<contours.size();i++){
            cv::Rect rect=cv::boundingRect(contours[i]);
            cv::rectangle(imagen, rect, CV_RGB(255,0,0));
        }

        QPixmap pixmap;
        pixmap=pixmap.fromImage(QtOcv::mat2Image(imagen));

  ((QLabel*)ui->gridLayout->itemAt(pos)->widget())->setPixmap(pixmap);
  if(settings.value("transmitir")==true){
    //qDebug() << "CONNECT OK";
      if(conexion->isEncrypted())
        emitir(image,pos,contours);
  }
}
void ClienteV::error(QAbstractSocket::SocketError algo)
{


    conexion->ignoreSslErrors();
    int i= algo;
    qDebug() << "que Error"<< conexion->errorString();
    qDebug() <<"error"<< i;
};
