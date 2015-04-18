#include "servidor.h"


Servidor::Servidor():
server(NULL),
Vdb(NULL)
{
    server= new QTcpServer;
    server->setMaxPendingConnections(10);
    Vdb= new QSqlDatabase();
    Vdb->addDatabase("QSQLITE");
    Vdb->setDatabaseName("videovigilancia.sqlite");
    bool algo = Vdb->open();
    if (!algo) {
        qDebug() << Vdb->lastError().text();;
        qDebug() << QSqlDatabase::drivers();
        //exit(1);
    }
    QSqlQuery query(*Vdb);
    query.exec("CREATE TABLE IF NOT EXISTS REGVAF "
                "(PRO VARCHAR(5),"
                " V VARBINARY(1),"
                " NCAMARA VARCHAR(60) PRIMARY KEY,"
                " NPC VARCHAR(60) PRIMARY KEY,"
                " TIMESTAMP VARCHAR(30) PRIMARY KEY "
                " DIRECTORIO VARCHAR(500))");
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
        client *nuevaC =new client(clientConnection,Vdb,this);
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
