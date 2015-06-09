#ifndef SERVIDORVSINCRONO_H
#define SERVIDORVSINCRONO_H

#include <QObject>

#include "server.h"
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSettings>
class servidorvsincrono : public QObject
{
    Q_OBJECT
public:
    explicit servidorvsincrono(QString RutadatosVariables,QString Rutacertificadoclave,int Puerto,QObject *parent = 0);
    ~servidorvsincrono();
    void inicioServer();

    void OpcionesLimpieza();
signals:

public slots:


private:
    server *serverS;
    QSqlDatabase Vdb;
    QSettings *rutas;
};

#endif // SERVIDORVSINCRONO_H
