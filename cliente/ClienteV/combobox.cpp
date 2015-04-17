#include "combobox.h"
#include "ui_combobox.h"

combobox::combobox(const QList<QByteArray> &devices,QWidget * parent) :
    QDialog(parent),
    ui(new Ui::combobox)
{
    ui->setupUi(this);
    int i=0;

    do {
        ui->comboBox->addItem(QCamera::deviceDescription(devices[i]));
        i++;
    }while(i<devices.size());

    ui->comboBox->setCurrentIndex(settings.value("indice").toInt());
    ui->PORT->setText(settings.value("PORT").toString());
    ui->IP->setText(settings.value("IP").toString());
}

combobox::~combobox()
{
    delete ui;
}


void combobox::on_OkButton_clicked()
{
    int indice, puerto;
    QString IP;

    indice=ui->comboBox->currentIndex();
    puerto=ui->PORT->text().toInt();
    IP=ui->IP->text();

    settings.setValue("indice",indice);
    settings.setValue("PORT",puerto);
    settings.setValue("IP",IP);

    emit s_camera(indice);
    emit s_port(puerto);
    emit s_IP(IP);

    close();
}

void combobox::on_CerrarButton_clicked()
{
    close();
}
