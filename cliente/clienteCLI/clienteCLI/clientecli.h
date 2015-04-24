#ifndef CLIENTECLI_H
#define CLIENTECLI_H

#include <VAF.pb.h>
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


#define NPROTOCOLO "VAF"
#define VPROTOCOLO "1"

struct CAM{
    QCamera *Camera = NULL;
    CaptureBuffer *captureBuffer = NULL;
};

class ClienteCLI: public QObject
{
    Q_OBJECT

public:
    ClienteCLI();
    ~ClienteCLI();

    void debug();

    void actualizar_cam(int,bool);

    void actualizar_puerto(int puerto);

    void actualizar_IP(QString IP);

    void actualizar();

    void capturar();

private slots:

   void emitir(const QImage &image, int id);

private:
    int NCamaras=-1;
    QSettings settings;
    QVector<CAM> *ListaCamaras;
    QList<QByteArray> devices;
    QTcpSocket *conexion;
    std::string nombrePC;
};

#endif // CLIENTECLI_H
