#include "clientev.h"
#include "ui_clientev.h"

typedef std::vector<cv::Mat> ImagesType;
typedef std::vector<std::vector<cv::Point> > ContoursType;

ClienteV::ClienteV(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClienteV),
    backgroundSubtractor(1000,15,false)
{
    //this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->setupUi(this);

    if(settings.value("transmitir")==true)
        ui->CheckTransmitir->setChecked(true);

    devices = QCamera::availableDevices();
    NCamaras=devices.size();
    ListaCamaras=new QVector<CAM>;
    conexion=new QTcpSocket;


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

    if(settings.value("transmitir")==true)
        conexion->connectToHost(settings.value("IP").toString(),settings.value("PORT").toInt());

    //qDebug() << NCamaras;

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
            ui->MatrizCamaras->addWidget(label,i,j);
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

    if(settings.value("transmitir")==true)
        conexion->connectToHost(settings.value("IP").toString(),settings.value("PORT").toInt());

    //qDebug() << NCamaras;

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
            ui->MatrizCamaras->addWidget(label,i,j);
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

void ClienteV::emitir(const QImage &image, const int &pos){
    /*
    required string protocolo = 1;
    required bytes  version = 2;
    optional uint32  Tnombrecamara = 3 ;
    required string  nombrecamara = 4;
    optional uint32  TnombrePC = 5 ;
    required string  nombrePC = 6;
    required string  timestamp = 7;
    optional uint32  TImagen=8;
    required string  imagen=9;
    */
    QBuffer buffer;
    QImageWriter writer;
    std::string spaquete;
    VAF paquete;

    paquete.set_protocolo(NPROTOCOLO);
    paquete.set_version(VPROTOCOLO);

    //QString dprotocolo(paquete.protocolo().c_str());
    //QString diversion(paquete.version().c_str());
    //qDebug() << dprotocolo << diversion;

    std::string nombrecamara((QCamera::deviceDescription(devices[ListaCamaras->value(pos).id])).toStdString());
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

    //paquete.set_datestamp((QDate::currentDate().toString("dd.MM.yyyy")).toStdString());

    paquete.set_datestamp((QDate::currentDate().toString("dd.MM.yyyy")).toStdString());

    writer.setDevice(&buffer);
    writer.setFormat("jpeg");
    writer.setCompression(70);
    writer.write(image);
    QByteArray bimagen = buffer.buffer();
    qDebug()<< "imagen:"<< bimagen.size();
    //QString imagen(bimagen);
    paquete.set_timagen(bimagen.size());
    paquete.set_imagen(bimagen.data(),bimagen.size());

    //qint32 dtimagen((paquete.timagen()));
    //QString dimagen(paquete.imagen().c_str());
    //qDebug() << dimagen << dtimagen;

    paquete.SerializeToString(&spaquete);

    QByteArray bpaquete(spaquete.c_str(),spaquete.size());
    qint32 tbpaquete = bpaquete.size();
    qDebug() << "TBSIZE: " << tbpaquete;
    //QByteArray btbpaquete;
    //btbpaquete.append((const char*)&tbpaquete,sizeof(qint32));
//    btbpaquete.append('\n');
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    //qDebug() << "Size: " << btbpaquete.toInt() << "Paquete: " << bpaquete;
    out << (quint32)tbpaquete;
    //out << tbpaquete;
    //out.device()->seek(0);
    //conexion->write(btbpaquete);
    conexion ->write(block);
    //conexion->write("\n");
    //qDebug() << "sizeof mandado OK";
    conexion->write(bpaquete);
    //qDebug() << "bpaquete mandado OK";

}

void ClienteV::image_s(const QImage &image, const int &pos)
{
    // Instancia de la clase del sustractor de fondo
    // cv::BackgroundSubtractorMOG2(history=500,
    //                              varThreshold=16,
    //                              bShadowDetection=true)
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
    //}

  QPixmap pixmap;
  pixmap=pixmap.fromImage(QtOcv::mat2Image(imagen));
  //qDebug() << "MOSTRAR EN " << pos;
  ((QLabel*)ui->MatrizCamaras->itemAt(pos)->widget())->setPixmap(pixmap);
  if(settings.value("transmitir")==true){
    //qDebug() << "CONNECT OK";
    emitir(image,pos);
  }
}
