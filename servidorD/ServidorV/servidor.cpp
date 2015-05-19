#include "servidor.h"


void Servidor::OpcionesLimpieza()
{
    int limpieza=1;
    //syslog(LOG_ERR, "LIMPIANDO");
    //std::cout <<"Desea hacer limpieza del directorio variable(1 caso afirmativo)"<< std::endl;
    //std::cin >> limpieza;
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
    }
    //syslog(LOG_ERR, "LIMPIO!");
}

Servidor::Servidor():
server(NULL)
{
    syslog(LOG_ERR, "CONSTRUYENDO UNA COSA GUAPA AHI");
    server= new QTcpServer;
    server->setMaxPendingConnections(10);
    syslog(LOG_ERR, "CHACHO Y LAS CONEXIONES");
    QDir directorio;
    Vdb=QSqlDatabase::addDatabase("QSQLITE", "SQLITE");
    directorio.mkpath(QString(APP_VARDIR)+"/BDD");
    directorio.mkpath(QString(APP_VARDIR)+ "/clientes");
    Vdb.setDatabaseName(QString(APP_VARDIR)+"/BDD/"+"videovigilancia.sqlite");
    bool algo = Vdb.open();
    if (!algo) {
        syslog(LOG_ERR, "ERROR AL ABRIR LA BBDD");
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
    syslog(LOG_ERR, "CHAS ESA BASEDATOS ES LA");

}

void Servidor::inicioServer()
{
    server->listen(QHostAddress::AnyIPv4,33333);
    //syslog(LOG_ERR, "LISEN TU MI");
    connect(server,SIGNAL(newConnection()),this,SLOT(conexionesPen()));

}

void Servidor::conexionesPen()
{
    //syslog(LOG_ERR, "CONEXIONESPEN");
    while(server->hasPendingConnections() && clients.size()<10) {
        QTcpSocket *clientConnection =
            server->nextPendingConnection();
        client *nuevaC =new client(clientConnection,Vdb,this);
        clients[nuevaC->getsocketDescriptor()] = nuevaC;

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
    //syslog(LOG_ERR, "ELIMINAR LISTA");
    QMap<qintptr, client*>::iterator i/*= clients.find(sock)*/;

    for(i=clients.begin();i != clients.end();i++){
        qDebug() <<clients.size() <<i.value()->get_tcp()->isValid();
        if (i.value()->get_tcp()->isValid()==false){
            clients.erase(i);
            qDebug() <<clients.size();
            return true;
        }
    }
    return false;

}
