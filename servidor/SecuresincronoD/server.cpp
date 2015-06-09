#include "server.h"


server::server(QSqlDatabase &bdd,QString RutadatosVariables,QString Rutacertificadoclave, QObject *parent):QTcpServer(parent),
    bddc(bdd)
{



    Rutadatos=RutadatosVariables;
    Rutacertificado=Rutacertificadoclave;
    syslog(LOG_ERR, "SERVIDOR CREADO");
    if (!bddc.open()) {
       qDebug() <<"No se pudo acceder a los datos";
       exit(1);
    }

}


void server::incomingConnection(qintptr socketDescriptor)
{
        syslog(LOG_ERR, "CREANDO CLIENTE....");
        clienteT *thread = new clienteT(socketDescriptor,bddc,Rutadatos,Rutacertificado, this);
        syslog(LOG_ERR, "CREADO CLIENTE");
        //connect(thread, SIGNAL(finished()), this, SLOT(eliminarlista()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        //clients[socketDescriptor] = thread;
        qDebug() <<"NuevoCliente" <<socketDescriptor;
        //qDebug() <<clients.size();
        thread->start();

}


