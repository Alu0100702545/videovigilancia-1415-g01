#include "servidor.h"


Servidor::Servidor():
server(NULL)
{
    server= new QTcpServer;
    server->setMaxPendingConnections(10);

}

void Servidor::inicioServer()
{
    server->listen(QHostAddress::AnyIPv4,33333);
    connect(server,SIGNAL(newConnection()),this,SLOT(conexionesPen()));

}

void Servidor::conexionesPen()
{
    while(server->hasPendingConnections() && clients.size()<10) {
        QTcpSocket *clientConnection =
            server->nextPendingConnection();
        client *nuevaC =new client(clientConnection,this);
        clients[nuevaC->getsocketDescriptor()] = nuevaC;
        connect(nuevaC,SIGNAL(eliminar(qintptr)),this, SLOT(eliminarlista(qintptr)));


    }
    if(clients.size()>=10)
        server->close();
    if (!server->isListening()&& clients.size() <10){
        server->listen(QHostAddress::AnyIPv4,33333);
        connect(server,SIGNAL(newConnection()),this,SLOT(conexionesPen()));
    }
}

void Servidor::eliminarlista(qintptr sock)
{

    QMap<qintptr, client*>::iterator i= clients.find(sock);
    if (i != clients.end())
        clients.erase(i);
}
