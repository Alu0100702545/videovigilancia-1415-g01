#include "servidor.h"


void Servidor::OpcionesLimpieza()
{
    int limpieza=1;
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
            if (all_dirs.back()== APP_VARDIR)
                all_dirs.pop_back();
            else{
            directorio.rmpath(all_dirs.back());
            all_dirs.pop_back();
            }
        }
     //exit(0);
    }
}

Servidor::Servidor():
server(NULL)
{

    if (settings.value("PORT").isNull())
        settings.setValue("PORT",33333);

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
    server= new QSslServer(this,Vdb);
    server->setMaxPendingConnections(10);
    QSqlQuery query(Vdb);

    query.exec("CREATE TABLE IF NOT EXISTS regvaf "
               "("
               " PRO VARCHAR(5),"
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
               " ANCHO INT(4),"
               " ALTO  INT(4),"
               " CRX  INT(4),"
               " CRY  INT(4),"
               " PRIMARY KEY (DIRECTORIO,ANCHO,ALTO,CRX,CRY),"
               " FOREIGN KEY (DIRECTORIO) REFERENCES regvaf ON DELETE CASCADE)");



    QStringList table=Vdb.tables();
    qDebug() << table;

}

void Servidor::inicioServer()
{
    server->listen(QHostAddress::AnyIPv4,settings.value("PORT").toInt());
    //connect(server,SIGNAL(newConnection()),this,SLOT(conexionesPen()));
    //connect(server,SIGNAL(newConnection()),this,SLOT(fallos()));
    qDebug() << "ESCUCHANDO";

}

void Servidor::actualizar_puerto(int puerto)
{
    settings.setValue("PORT",puerto);
    server->close();

    server->listen(QHostAddress::AnyIPv4,settings.value("PORT").toInt());


}



