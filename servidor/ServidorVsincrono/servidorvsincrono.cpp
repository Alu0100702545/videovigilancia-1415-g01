#include "servidorvsincrono.h"

servidorvsincrono::servidorvsincrono(QObject *parent) : QObject(parent),serverS(NULL)
{

    //server->setMaxPendingConnections(10);

    QDir directorio;
    Vdb=QSqlDatabase::addDatabase("QSQLITE", "SQLITE");
    //directorio.mkpath(QString(APP_VARDIR)+"/BDD");
    //directorio.mkpath(QString(APP_VARDIR)+ "/clientes");
    //Vdb.setDatabaseName(QString(APP_VARDIR)+"/BDD/"+"videovigilancia.sqlite");
    Vdb.setDatabaseName("videovigilancia.sqlite");
    bool algo = Vdb.open();
    if (!algo) {
        //qDebug() << Vdb.lastError();
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
    serverS= new server(Vdb,this);
}

servidorvsincrono::~servidorvsincrono()
{

}

void servidorvsincrono::inicioServer()
{
    serverS->listen(QHostAddress::AnyIPv4,33333);
    //connect(server,SIGNAL(newConnection()),this,SLOT(conexionesPen()));

}
