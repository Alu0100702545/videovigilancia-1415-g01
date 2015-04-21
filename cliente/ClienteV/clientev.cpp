#include "clientev.h"
#include "ui_clientev.h"

ClienteV::ClienteV(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClienteV)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
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
    qDebug() << "NombrePC: "<< NnombrePC<< " size: " << sizeof(nombrePC);;


}

ClienteV::~ClienteV()
{
    delete ui;
}

void ClienteV::on_BotonCerrar_clicked()
{
    qApp->quit();
}

void ClienteV::on_actionCerrar_triggered()
{
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
    qDebug() << "ACT NAMESETTING: " << namesetting;
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
    int NLabels=0;

    if(settings.value("transmitir")==true)
        conexion->connectToHost(settings.value("IP").toString(),settings.value("PORT").toInt());

    if(NCamaras%2==0)
        NLabels=NCamaras;
    else
        NLabels=NCamaras+1;

    for(int i=0;i<NLabels/2;i++){
        for(int j=0;j<NLabels/2;j++){
            QLabel* label=new QLabel;
            label->setScaledContents(true);
            ui->gridLayout->addWidget(label,i,j);
            qDebug() << "AÑADIDO EN " << i << j;
        }
    }

    for(int i=0;i<NCamaras;i++)
    {
        namesetting="indice";
        namesetting.append(i+48);
        qDebug() << "CAPT NAMESETTING: " << namesetting;
        if((settings.value(namesetting))==true)
        {
            qDebug() << "ENTRAMOS CON " << namesetting;
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
    int NLabels=0;

    if(settings.value("transmitir")==true)
        conexion->connectToHost(settings.value("IP").toString(),settings.value("PORT").toInt());

    if(NCamaras%2==0)
        NLabels=NCamaras;
    else
        NLabels=NCamaras+1;

    for(int i=0;i<NLabels/2;i++){
        for(int j=0;j<NLabels/2;j++){
            QLabel* label=new QLabel;
            label->setScaledContents(true);
            ui->gridLayout->addWidget(label,i,j);
            qDebug() << "AÑADIDO EN " << i << j;
        }
    }

    for(int i=0;i<NCamaras;i++)
    {
        namesetting="indice";
        namesetting.append(i+48);
        qDebug() << "CAPT NAMESETTING: " << namesetting;
        if((settings.value(namesetting))==true)
        {
            qDebug() << "ENTRAMOS CON " << namesetting;
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

    QByteArray bytes, bimagen;
    QBuffer buffer(&bytes);
    QImageWriter writer;
    std::string spaquete;
    VAF paquete;

    paquete.set_protocolo(NPROTOCOLO);
    paquete.set_version(VPROTOCOLO);

    std::string nombrecamara((QCamera::deviceDescription(devices[ListaCamaras->value(pos).id])).toStdString());
    paquete.set_tnombrecamara(sizeof(nombrecamara));
    paquete.set_nombrecamara(nombrecamara);

    paquete.set_tnombrepc(sizeof(nombrePC));
    paquete.set_nombrepc(nombrePC);

    paquete.set_timestamp((QTime::currentTime().toString("hh:mm:ss")).toStdString());

    writer.setDevice(&buffer);
    writer.setFormat("jpeg");
    writer.setCompression(70);
    writer.write(image);
    bimagen = buffer.buffer();
    QString imagen(bimagen);
    paquete.set_timagen(sizeof(imagen.toStdString()));
    paquete.set_imagen(imagen.toStdString());

    paquete.SerializeToString(&spaquete);
    QByteArray bpaquete(spaquete.c_str(),sizeof(spaquete.c_str()));
    qint32 tbpaquete = bpaquete.size();
    QByteArray btbpaquete;
    btbpaquete.number(tbpaquete);

    conexion->write(btbpaquete);
    qDebug() << "sizeof mandado OK";
    //conexion->write(bpaquete);
    qDebug() << "bpaquete mandado OK";

}

void ClienteV::image_s(const QImage &image, const int &pos)
{

  QPixmap pixmap;
  pixmap=pixmap.fromImage(image);
  //qDebug() << "MOSTRAR EN " << id;
  ((QLabel*)ui->gridLayout->itemAt(pos)->widget())->setPixmap(pixmap);
  if(settings.value("transmitir")==true){
    qDebug() << "CONNECT OK";
    emitir(image,pos);
  }
}
