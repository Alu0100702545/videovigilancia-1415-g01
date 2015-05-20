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
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QTime>
#include <QPainter>
#include <capturebuffer.h>
#include <QTcpSocket>
#include <QBuffer>
#include <QImageWriter>
#include <QHostInfo>
#include <QSocketNotifier>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <QSslSocket>
#include <cvmatandqimage.h>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


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
    bool parametros=false;

    ClienteCLI();

    ClienteCLI(int argc, char *argv[]);

    ~ClienteCLI();

    void debug();

    void actualizar_cam(int,bool);

    void actualizar_puerto(int puerto);

    void actualizar_IP(QString IP);

    void actualizar();

    void capturar();

    void delete_all();

    void termSignalHandler(int unused);


private slots:

   void emitir(const QImage &image, int id);

public slots:
   void handleSigTerm();

private:
    int NCamaras=-1;
    QSettings settings;
    QVector<CAM> *ListaCamaras;
    QList<QByteArray> devices;
    QSslSocket *conexion;
    QString* NombrePC;
    int sigTermSd[2];
    QSocketNotifier *sigTermNotifier;
    cv::BackgroundSubtractorMOG2 backgroundSubtractor;
};

#endif // CLIENTECLI_H
