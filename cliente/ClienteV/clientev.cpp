#include "clientev.h"
#include "ui_clientev.h"

ClienteV::ClienteV(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClienteV)
{
    ui->setupUi(this);

    if(settings.value("transmitir")==true)
        ui->CheckTransmitir->setChecked(true);

    devices = QCamera::availableDevices();
    captureBuffer = new CaptureBuffer;
    movie = NULL;
    camera = NULL;

    conexion=new QTcpSocket;

    connect(captureBuffer,SIGNAL(s_image(QImage)),this,SLOT(image_s(QImage)));
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
    connect(preferencias,SIGNAL(s_camera(int)),this,SLOT(actualizar_cam(int)));
    connect(preferencias,SIGNAL(s_puerto(int)),this,SLOT(actualizar_puerto(int)));
    connect(preferencias,SIGNAL(s_IP(QString)),this,SLOT(actualizar_IP(QString)));
}

void ClienteV::actualizar_cam(int i)
{
    settings.setValue("indice",i);
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
    camera = new QCamera(devices[settings.value("indice").toInt()]); // no permite capturar de la cam

    camera->setViewfinder(captureBuffer);

    camera->start();
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

void ClienteV::on_actionCapturar_triggered()
{
    qDebug() << "Capturar!";
    camera = new QCamera(devices[settings.value("indice").toInt()]); // no permite capturar de la cam

    camera->setViewfinder(captureBuffer);

    camera->start();
    qDebug() << "Inicio camara";

}

void ClienteV::image_s(const QImage &image)
{

  QPixmap pixmap;
  pixmap=pixmap.fromImage(image);

  ui->label->setPixmap(pixmap);

  if(settings.value("transmitir")==true)
    emitir(image);
}

void ClienteV::video_s(const QRect &rect)
{
  if(settings.value("transmitir")==true)
    emitir(movie->currentImage());
}
