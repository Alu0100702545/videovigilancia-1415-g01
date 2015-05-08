#include "servidor.h"


void Servidor::OpcionesLimpieza()
{
    int limpieza=0;
    std::cout <<"Desea hacer limpieza del directorio variable(1 caso afirmativo)"<< std::endl;
    std::cin >> limpieza;
    QDir directorio;
    if (limpieza==1){
    //qDebug() << ;
        QSqlQuery query(Vdb);
        query.exec("Select DIRECTORIO from regvaf");
        while (query.next()) {
                QString nombre = query.value("DIRECTORIO").toString();
                //qDebug() << nombre;
               directorio.remove(nombre);

        }
        query.exec("DROP TABLE regvaf");
        directorio.remove(QString(APP_VARDIR)+"/BDD/"+"videovigilancia.sqlite");
        QStringList all_dirs;
        all_dirs << APP_VARDIR;
        QDirIterator directories(APP_VARDIR, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while(directories.hasNext()){
            directories.next();
            all_dirs << directories.filePath();
        }
        //qDebug()<< all_dirs;
        while(!all_dirs.empty()){
            directorio.rmpath(all_dirs.back());
            all_dirs.pop_back();

        }
     exit(0);
    }
}

Servidor::Servidor():
server(NULL)
{
    server= new QTcpServer;
    server->setMaxPendingConnections(10);

    QDir directorio;
    Vdb=QSqlDatabase::addDatabase("QSQLITE", "SQLITE");
    directorio.mkpath(QString(APP_VARDIR)+"/BDD");
    directorio.mkpath(QString(APP_VARDIR)+ "/clientes");
    Vdb.setDatabaseName(QString(APP_VARDIR)+"/BDD/"+"videovigilancia.sqlite");
    bool algo = Vdb.open();
    if (!algo) {
        qDebug() << Vdb.lastError().text();
        qDebug() << QSqlDatabase::drivers();
        exit(1);
    }
    QSqlQuery query(Vdb);

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
        QSslSocket *cliente= (QSslSocket*)clientConnection;
        cliente->setLocalCertificate("videovigilancia.pem");
        cliente->setPrivateKey("videovigilancia.key");
        qDebug() <<"certificado valido ?: "<<cliente->localCertificate().isNull();
        /*QSslKey eso=cliente->privateKey();
        qDebug() <<"soy valido ?: "<<eso;
        */

        client *nuevaC =new client(cliente,Vdb,this);
        clients[nuevaC->getsocketDescriptor()] = nuevaC;
        qDebug() <<"soy valido ?: "<<cliente->isValid();
        qDebug() <<"estoy encriptado ?: "<<cliente->isEncrypted();
        qDebug() << "nuevaConexion";
        qDebug() << nuevaC;
        //qDebug() <<clients.size();

    }
    if(clients.size()>=10)
        if(eliminarlista()==false)
            server->close();
    if (!server->isListening()&& clients.size() <10){
        server->listen(QHostAddress::AnyIPv4,33333);
        connect(server,SIGNAL(newConnection()),this,SLOT(conexionesPen()));

    }
}

bool Servidor::eliminarlista()
{
    QMap<qintptr, client*>::iterator i/*= clients.find(sock)*/;

    for(i=clients.begin();i != clients.end();i++){
        qDebug() <<"pene"<<clients.size() <<i.value()->get_tcp()->isValid();
        if (i.value()->get_tcp()->isValid()==false){
            clients.erase(i);
            qDebug() <<clients.size();
            return true;
        }
    }
    return false;

}
