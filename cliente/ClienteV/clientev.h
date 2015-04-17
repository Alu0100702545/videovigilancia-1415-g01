#ifndef CLIENTEV_H
#define CLIENTEV_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraViewfinder>
#include <QSettings>
#include <QApplication>
#include <QLabel>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QMovie>
#include <QTime>
#include <QPainter>
#include <acercade.h>
#include <capturebuffer.h>
#include <combobox.h>
#include <QTcpSocket>
#include <QBuffer>
#include <QImageWriter>

namespace Ui {
class ProyectoVideo;
}

class ProyectoVideo : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProyectoVideo(QWidget *parent = 0);
    ~ProyectoVideo();

private slots:
    void on_BotonAbrir_clicked();

    void on_BotonReproducir_clicked();

    void on_BotonCerrar_clicked();

    void on_BotonParar_clicked();

    void on_CheckAutoPlay_clicked();

    void on_actionParar_triggered();

    void on_actionAbrir_triggered();

    void on_actionCerrar_triggered();

    void on_actionReproducir_triggered();

    void on_actionProyecto_Video_triggered();

    void on_actionOpciones_triggered();

    void actualizar_cam(int);

    void on_BotonCapturar_clicked();

    void on_actionCapturar_triggered();

    void image_s(const QImage &);

    void emitir(const QImage &image);

    void actualizar_puerto(int puerto);

    void actualizar_IP(QString IP);

    void on_BotonPausa_clicked();

    void on_CheckTransmitir_clicked();
    void video_s(const QRect &rect);
private:
    bool pause=false;
    int frame=0;
    Ui::ProyectoVideo *ui;
    QMovie *movie;
    QSettings settings;
    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QList<QByteArray> devices;
    CaptureBuffer *captureBuffer;
    QTcpSocket *conexion;
};

#endif // CLIENTEV_H
