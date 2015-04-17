#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QDialog>
#include <QSettings>
#include <QCamera>

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
     void s_camera(int);
     void s_port(int);
     void s_IP(QString);

private slots:
    void on_OkButton_clicked();

    void on_CerrarButton_clicked();

private:
    Ui::combobox *ui;
    QSettings settings;
};

#endif // COMBOBOX_H
