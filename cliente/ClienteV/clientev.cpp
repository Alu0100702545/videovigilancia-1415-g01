#include "clientev.h"
#include "ui_clientev.h"

ProyectoVideo::ProyectoVideo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProyectoVideo)
{
    ui->setupUi(this);

    if(settings.value("autoplay")==true)
        ui->CheckAutoPlay->setChecked(true);

    if(settings.value("transmitir")==true)
        ui->CheckTransmitir->setChecked(true);

    devices = QCamera::availableDevices();
    captureBuffer = new CaptureBuffer;
    movie = NULL;
    camera = NULL;

    conexion=new QTcpSocket;

    connect(captureBuffer,SIGNAL(s_image(QImage)),this,SLOT(image_s(QImage)));
}

ProyectoVideo::~ProyectoVideo()
{
    delete ui;
}

void ProyectoVideo::on_BotonAbrir_clicked()
{
    if(movie!=NULL)
        movie->stop();

    if(camera!=NULL)
        camera->stop();

    conexion->connectToHost(settings.value("IP").toString(),settings.value("PORT").toInt());

    QString fileName =
            QFileDialog::getOpenFileName(this , tr("OpenFile"),
                                          QString(),tr("Video files(*avi *wmv *mjpeg)"));

            if (!fileName.isEmpty()){
              QFile file(fileName);
              if (!file.open(QIODevice::ReadOnly)){
                  QMessageBox::critical(this,tr("Error"),tr("no se puede abrir"));
                  return;
              }

              movie = new QMovie(fileName);
              movie->frameRect();
              connect(movie,SIGNAL(updated(QRect)),this,SLOT(video_s(QRect)));
              ui->label->setMovie(movie);
              if(settings.value("autoplay")==true)
                  movie->start();
            }
}

void ProyectoVideo::on_actionAbrir_triggered()
{
    if(movie!=NULL)
        movie->stop();

    if(camera!=NULL)
        camera->stop();

    QString fileName =
            QFileDialog::getOpenFileName(this , tr("OpenFile"),
                                          QString(),tr("Video files(*avi *wmv *mjpeg)"));

            if (!fileName.isEmpty()){
              QFile file(fileName);
              if (!file.open(QIODevice::ReadOnly)){
                  QMessageBox::critical(this,tr("Error"),tr("no se puede abrir"));
                  return;
              }

              movie = new QMovie(fileName);
              movie->frameRect();
              connect(movie,SIGNAL(updated(QRect)),this,SLOT(video_s(QRect)));
              ui->label->setMovie(movie);
              if(settings.value("autoplay")==true)
                  movie->start();
            }
}

void ProyectoVideo::on_BotonReproducir_clicked()
{
    if(movie!=NULL)
            movie->start();
}

void ProyectoVideo::on_actionReproducir_triggered()
{
    if(movie!=NULL)
            movie->start();
}

void ProyectoVideo::on_BotonPausa_clicked()
{
    if(movie!=NULL)
            movie->setPaused(true);

}

void ProyectoVideo::on_BotonParar_clicked()
{
    if(movie!=NULL)
        movie->stop();

    if(camera!=NULL)
        camera->stop();
}

void ProyectoVideo::on_actionParar_triggered()
{
    if(movie!=NULL)
        movie->stop();

    if(camera!=NULL)
        camera->stop();
}

void ProyectoVideo::on_BotonCerrar_clicked()
{
    qApp->quit();
}

void ProyectoVideo::on_actionCerrar_triggered()
{
    qApp->quit();
}

void ProyectoVideo::on_CheckAutoPlay_clicked()
{
    if(settings.value("autoplay")==false)
        settings.setValue("autoplay" ,true);
    else
        settings.setValue("autoplay" ,false);
}

void ProyectoVideo::on_CheckTransmitir_clicked()
{
    if(settings.value("transmitir")==false)
        settings.setValue("transmitir" ,true);
    else
        settings.setValue("transmitir" ,false);
}

void ProyectoVideo::on_actionProyecto_Video_triggered()
{
    AcercaDe *about;
    about= new AcercaDe;
    about->show();
}

void ProyectoVideo::on_actionOpciones_triggered()
{
    combobox* preferencias=new combobox(devices);
    preferencias->show();
    connect(preferencias,SIGNAL(s_camera(int)),this,SLOT(actualizar_cam(int)));
    connect(preferencias,SIGNAL(s_puerto(int)),this,SLOT(actualizar_puerto(int)));
    connect(preferencias,SIGNAL(s_IP(QString)),this,SLOT(actualizar_IP(QString)));
}

void ProyectoVideo::actualizar_cam(int i)
{
    settings.setValue("indice",i);
}

void ProyectoVideo::actualizar_puerto(int puerto)
{
    settings.setValue("puerto",puerto);
}

void ProyectoVideo::actualizar_IP(QString IP)
{
    settings.setValue("IP",IP);
}

void ProyectoVideo::on_BotonCapturar_clicked()
{
    camera = new QCamera(devices[settings.value("indice").toInt()]); // no permite capturar de la cam

    camera->setViewfinder(captureBuffer);

    camera->start();
}

void ProyectoVideo::emitir(const QImage &image){
    QBuffer buffer;
    QImageWriter writer;
    writer.setDevice(&buffer);
    writer.setFormat("jpeg");
    writer.setCompression(70);
    writer.write(image);
    QByteArray bites=buffer.buffer();
    conexion->write(bites);
}

void ProyectoVideo::on_actionCapturar_triggered()
{
    qDebug() << "Capturar!";
    camera = new QCamera(devices[settings.value("indice").toInt()]); // no permite capturar de la cam

    camera->setViewfinder(captureBuffer);

    camera->start();
    qDebug() << "Inicio camara";

}

void ProyectoVideo::image_s(const QImage &image)
{

  QPixmap pixmap;
  pixmap=pixmap.fromImage(image);

  ui->label->setPixmap(pixmap);

  if(settings.value("transmitir")==true)
    emitir(image);
}

void ProyectoVideo::video_s(const QRect &rect)
{
  if(settings.value("transmitir")==true)
    emitir(movie->currentImage());
}
