#include "servidor.h"


Servidor::Servidor():
server(NULL)
{
    server= new QTcpServer;
    server->setMaxPendingConnections(10);

    //Vdb= new QSqlDatabase();

    Vdb=QSqlDatabase::addDatabase("QSQLITE", "SQLITE");
    //QSqlDatabase vvdb = QSqlDatabase::addDatabase("QSQLITE", "SQLITE");

    qDebug() << Vdb.connectionName();
    qDebug() << Vdb.driverName();
    qDebug() << Vdb.isDriverAvailable("QSQLITE");
    qDebug() << Vdb.isValid();
    Vdb.setDatabaseName("videovigilancia.sqlite");
    bool algo = Vdb.open();
    if (!algo) {
        qDebug() << Vdb.lastError().text();
        qDebug() << QSqlDatabase::drivers();
        //exit(1);
    }
    QSqlQuery query(Vdb);

    QDir directorio;
    directorio.remove("cosa/*.jpeg");
    qDebug() << query.exec("Select DIRECTORIO from regvaf");
     QString nombre;
    while (query.next()) {
                nombre = query.value("DIRECTORIO").toString();
                qDebug() << nombre;
               directorio.remove(nombre);
    }

    directorio.cleanPath("cosa");
    directorio.rmpath(".");
    //directorio.mkpath("cosa");
    query.exec("DROP TABLE regvaf");
    query.exec("CREATE TABLE IF NOT EXISTS regvaf "
               "(PRO VARCHAR(5),"
               " V VARBINARY(1),"
               " NCAMARA VARCHAR(60),"
               " NPC VARCHAR(60),"
               " DATESTAMP VARCHAR(30),"
               " TIMESTAMP VARCHAR(30), "
               " DIRECTORIO VARCHAR(2000) DEFAULT NULL,"
               " PRIMARY KEY (NCAMARA,NPC,DATESTAMP,TIMESTAMP))");

    QStringList table=Vdb.tables();
    qDebug() << table;

   /* query.exec("CREATE TABLE IF NOT EXISTS contactos "
                "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                " nombre VARCHAR(50))");


    query.prepare("INSERT INTO contactos (nombre) "
                  "VALUES (:nombre)");
    query.bindValue(":nombre", "Jesús");
    query.exec();
    int contactoId = query.lastInsertId().toInt();
    qDebug() << contactoId;
    query.exec("SELECT * FROM contactos");
    while (query.next()) {
        QString nombre = query.value(1).toString();
        qDebug() << nombre;
    }
*/




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
        qDebug() << "nuevaConexion";
        qDebug() << nuevaC;

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
