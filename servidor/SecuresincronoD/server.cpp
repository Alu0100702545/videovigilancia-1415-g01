#include "server.h"


server::server(QSqlDatabase &bdd,QString RutadatosVariables,QString Rutacertificadoclave, QObject *parent):QTcpServer(parent),
    bddc(bdd)
{



    Rutadatos=RutadatosVariables;
    Rutacertificado=Rutacertificadoclave;
    if (!bddc.open()) {
       syslog(LOG_ERR, "NO SE PUDO ABRIR LA BDD . CERRANDO SERVIDOR");
       exit(1);
    }

}


void server::incomingConnection(qintptr socketDescriptor)
{

        clienteT *thread = new clienteT(socketDescriptor,bddc,Rutadatos,Rutacertificado, this);

        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();

}


