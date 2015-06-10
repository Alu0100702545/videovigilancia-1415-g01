#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QSettings>
#include "client.h"
#include "qsslserver.h"

class Servidor : public QObject
{
    Q_OBJECT
private:
    //QTcpSocket *cliente;
    QSslServer *server;
    QSqlDatabase Vdb;
    QMap<qintptr,client*> clients;
    QSettings settings;

    QSettings *rutas;
signals:

    void algo();

public:
    void OpcionesLimpieza();
    Servidor(QString RutadatosVariables,QString Rutacertificadoclave,int  Puerto);
    void inicioServer();


    void actualizar_puerto(int puerto);
};

#endif // SERVIDOR_H
