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
    int id=0;

    for(int i=0;i<NCamaras;i++){
            QLabel* label=new QLabel;
            label->setScaledContents(true);
            ui->gridLayout->addWidget(label);
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
            aux.captureBuffer->id=id;
            qDebug() << "BUFFER OK";
            ListaCamaras->push_back(aux);
            qDebug() << "PUSH OK";
            ListaCamaras->value(id).Camera->setViewfinder(ListaCamaras->value(id).captureBuffer);
            qDebug() << "SET OK";
            ListaCamaras->value(id).Camera->start();
            qDebug() << "START OK";
            connect(ListaCamaras->value(id).captureBuffer,SIGNAL(s_image(QImage,int)),this,SLOT(image_s(QImage,int)));
            id++;
        }
    }
}

void ClienteV::on_actionCapturar_triggered()
{
    CAM aux;
    QString namesetting;

    for(int i=0;i<NCamaras;i++){
            QLabel* label=new QLabel;
            label->setScaledContents(true);
            ui->gridLayout->addWidget(label);
    }

    for(int i=0;i<NCamaras;i++)
    {
        namesetting="indice";
        namesetting.append(i+48);

        if((settings.value(namesetting))==true)
        {
            aux.Camera=new QCamera(devices[i]);
            aux.captureBuffer=new CaptureBuffer;
            aux.captureBuffer->id=i;

            ListaCamaras->push_back(aux);
            ListaCamaras->value(i).Camera->setViewfinder(ListaCamaras->value(i).captureBuffer);
            ListaCamaras->value(i).Camera->start();

            connect(ListaCamaras->value(i).captureBuffer,SIGNAL(s_image(QImage,int)),this,SLOT(image_s(QImage,int)));
        }
    }
}

void ClienteV::emitir(const QImage &image){
    QBuffer buffer;
    QImageWriter writer;
    writer.setDevice(&buffer);
    writer.setFormat("jpeg");
    writer.setCompression(70);
    writer.write(image);
    QByteArray bites=buffer.buffer();
    conexion->write(bites);
}

void ClienteV::image_s(const QImage &image, const int &id)
{

  QPixmap pixmap;
  pixmap=pixmap.fromImage(image);
  ((QLabel*)ui->gridLayout->itemAt(id)->widget())->setPixmap(pixmap);
  if(settings.value("transmitir")==true)
    emitir(image);
}
