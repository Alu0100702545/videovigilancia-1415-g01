#include "servidorvsincrono.h"

servidorvsincrono::servidorvsincrono(QString RutadatosVariables,QString Rutacertificadoclave,int  Puerto, QObject *parent) : QObject(parent),serverS(NULL)
{

    //server->setMaxPendingConnections(10);
    QDir directorio;
    rutas = new QSettings;
    rutas->setValue("RUTADATOS",RutadatosVariables);
    rutas->setValue("RUTACERT",Rutacertificadoclave);
    rutas->setValue("PORT",Puerto);
    Vdb=QSqlDatabase::addDatabase("QSQLITE", "SQLITE");
    directorio.mkpath(rutas->value("RUTADATOS").toString()+"/BDD");
    directorio.mkpath(rutas->value("RUTADATOS").toString()+ "/clientes");
    Vdb.setDatabaseName(rutas->value("RUTADATOS").toString()+"/BDD/"+"videovigilancia.sqlite");
    //Vdb.setDatabaseName("videovigilancia.sqlite");
    bool algo = Vdb.open();
    if (!algo) {
        syslog(LOG_ERR, "NO SE PUDO ABRIR LA BDD . CERRANDO SERVIDOR");
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
    query.exec("CREATE TABLE IF NOT EXISTS ROI "
               "("
               " DIRECTORIO VARCHAR(2000),"
               " CX1 INT(4),"
               " CX2  INT(4),"
               " CY1  INT(4),"
               " CY2  INT(4),"
               " PRIMARY KEY (DIRECTORIO,CX1,CX2,CY1,CY2),"
               " FOREIGN KEY (DIRECTORIO) REFERENCES regvaf ON DELETE CASCADE)");

    QStringList table=Vdb.tables();
    qDebug() << table;
    serverS= new server(Vdb,rutas->value("RUTADATOS").toString(),rutas->value("RUTACERT").toString(),this);


}

servidorvsincrono::~servidorvsincrono()
{

}

void servidorvsincrono::inicioServer()
{

    serverS->listen(QHostAddress::AnyIPv4,rutas->value("PORT").toInt());
}
void servidorvsincrono::OpcionesLimpieza()
{
    int limpieza=1;
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
        directorio.remove(rutas->value("RUTADATOS").toString()+"/BDD/"+"videovigilancia.sqlite");
        QStringList all_dirs;
        all_dirs << rutas->value("RUTADATOS").toString();
        QDirIterator directories(rutas->value("RUTADATOS").toString(), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while(directories.hasNext()){
            directories.next();
            all_dirs << directories.filePath();
        }
        //qDebug()<< all_dirs;
        while(!all_dirs.empty()){
            if (all_dirs.back()== rutas->value("RUTADATOS").toString())
                all_dirs.pop_back();
            else{
            directorio.rmpath(all_dirs.back());
            all_dirs.pop_back();
            }
        }
     //exit(0);
    }
}
