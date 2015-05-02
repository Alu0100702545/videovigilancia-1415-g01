#ifndef SERVIDORVSINCRONO_H
#define SERVIDORVSINCRONO_H

#include <QObject>

#include "server.h"

class servidorvsincrono : public QObject
{
    Q_OBJECT
public:
    explicit servidorvsincrono(QObject *parent = 0);
    ~servidorvsincrono();
    void inicioServer();

signals:

public slots:


private:
    server *serverS;
    QSqlDatabase Vdb;
};

#endif // SERVIDORVSINCRONO_H
