#ifndef CLIENTEV_H
#define CLIENTEV_H
#include <VAF.pb.h>
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
#include <acercade.h>
#include <capturebuffer.h>
#include <combobox.h>
#include <QTcpSocket>
#include <QSslSocket>
#include <QBuffer>
#include <QImageWriter>
#include <QHostInfo>
#include <openssl/ssl.h>

#include <QDate>

#define NPROTOCOLO "VAF"
#define VPROTOCOLO "1"

struct CAM{
    QCamera *Camera = NULL;
    CaptureBuffer *captureBuffer = NULL;
    int id=-1;
};

namespace Ui {
class ClienteV;
}

class ClienteV : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClienteV(QWidget *parent = 0);
    ~ClienteV();

public slots:
    void error(QAbstractSocket::SocketError algo);
private slots:

    void on_BotonCerrar_clicked();

    void on_actionCerrar_triggered();

    void on_actionProyecto_Video_triggered();

    void on_actionOpciones_triggered();

    void actualizar_cam(int,bool);

    void on_BotonCapturar_clicked();

    void on_actionCapturar_triggered();

    void image_s(const QImage &, const int &);

    void emitir(const QImage &image, const int &pos);

    void actualizar_puerto(int puerto);

    void actualizar_IP(QString IP);

    void on_CheckTransmitir_clicked();

private:
    int NCamaras=-1;
    Ui::ClienteV *ui;
    QSettings settings;
    QVector<CAM> *ListaCamaras;
    QList<QByteArray> devices;
    QSslSocket *conexion;
    std::string nombrePC;
};

#endif // CLIENTEV_H
