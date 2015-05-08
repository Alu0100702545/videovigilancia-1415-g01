#include "combobox.h"
#include "ui_combobox.h"

combobox::combobox(const QList<QByteArray> &devices,QWidget * parent) :
    QDialog(parent),
    ui(new Ui::combobox)
{
    ui->setupUi(this);

    NCamaras=devices.size();

    for(int i=0;i<NCamaras;i++){
        QCheckBox* item=new QCheckBox(QCamera::deviceDescription(devices[i]));
        ui->verticalLayout->addWidget(item);
    }

    QString namesetting;

    for(int i=0;i<NCamaras;i++){
        namesetting="indice";
        namesetting.append(i+48);
        if(settings.value(namesetting)==true)
        {
            ((QCheckBox*)ui->verticalLayout->itemAt(i)->widget())->setChecked(true);
        }
        else
            ((QCheckBox*)ui->verticalLayout->itemAt(i)->widget())->setChecked(false);
    }

    ui->PORT->setText(settings.value("PORT").toString());
    ui->IP->setText(settings.value("IP").toString());
}

combobox::~combobox()
{
    delete ui;
}

void combobox::on_OkButton_clicked()
{
    int puerto;
    QString IP;
    QString namesetting;
    //int indice;

    for(int i=0; i<NCamaras;i++){
        namesetting="indice";
        namesetting.append(i+48);
        qDebug() << "OK NAMESETTING: " << namesetting;
        if(((QCheckBox*)ui->verticalLayout->itemAt(i)->widget())->isChecked()){
            settings.setValue(namesetting,true);
            emit s_camera(i+48,true);
        }
        else{
            settings.setValue(namesetting,false);
            emit s_camera(i+48,false);
        }
    }

    //qDebug() << indice;

    puerto=ui->PORT->text().toInt();
    IP=ui->IP->text();

    settings.setValue("PORT",puerto);
    settings.setValue("IP",IP);

    emit s_port(puerto);
    emit s_IP(IP);

    close();
}

void combobox::on_CerrarButton_clicked()
{
    close();
}
