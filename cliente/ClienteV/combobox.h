#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QDialog>
#include <QSettings>
#include <QCamera>
#include <QtWidgets>
#include <QVector>

namespace Ui {
class combobox;
}

class combobox : public QDialog
{
    Q_OBJECT

public:
    explicit combobox(const QList<QByteArray> &devices,QWidget * parent=0);
    ~combobox();

signals:
     void s_camera(int,bool);
     void s_port(int);
     void s_IP(QString);

private slots:
    void on_OkButton_clicked();

    void on_CerrarButton_clicked();

private:
    Ui::combobox *ui;
    QSettings settings;
    int NCamaras=0;
};

#endif // COMBOBOX_H
