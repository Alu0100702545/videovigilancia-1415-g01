#ifndef CLIENTECLI_H
#define CLIENTECLI_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraViewfinder>
#include <QSettings>
#include <QApplication>
#include <QLabel>
#include <QTextStream>
#include <QVector>
#include <QString>
#include <QMessageBox>
#include <QTime>
#include <QPainter>
#include <capturebuffer.h>
#include <QTcpSocket>
#include <QBuffer>
#include <QImageWriter>
#include <QHostInfo>
#include <VAF.pb.h>

#define NPROTOCOLO "VAF"
#define VPROTOCOLO "1"

struct CAM{
    QCamera *Camera = NULL;
    CaptureBuffer *captureBuffer = NULL;
};

class ClienteCLI
{
public:
    ClienteCLI();
    ~ClienteCLI();

private slots:
    void goku();

    void actualizar();

    void actualizar_cam(int,bool);

    void image_s(const QImage &, const int &);

    void emitir(const QImage &image, const int &pos);

    void actualizar_puerto(int puerto);

    void actualizar_IP(QString IP);

private:
    int NCamaras=-1;
    QSettings settings;
    QVector<CAM> *ListaCamaras;
    QList<QByteArray> devices;
    QTcpSocket *conexion;
    std::string nombrePC;
};

#endif // CLIENTECLI_H
