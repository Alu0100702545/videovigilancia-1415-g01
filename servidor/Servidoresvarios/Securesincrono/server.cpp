#include "server.h"


server::server(QSqlDatabase &bdd, QObject *parent):QTcpServer(parent),
    bddc(bdd)
{
    if (!bddc.open()) {
       qDebug() <<"No se pudo acceder a los datos";
       exit(1);
    }

}


void server::incomingConnection(qintptr socketDescriptor)
{
        clienteT *thread = new clienteT(socketDescriptor,bddc, this);
        //connect(thread, SIGNAL(finished()), this, SLOT(eliminarlista()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        //clients[socketDescriptor] = thread;
        qDebug() <<"NuevoCliente" <<socketDescriptor;
        //qDebug() <<clients.size();
        thread->start();

}


