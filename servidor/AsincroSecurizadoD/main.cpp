#include <QDebug>
#include <QCoreApplication>
#include "servidor.h"
#include <QRegExp>
#include <syslog.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <grp.h>

/*void Signal_Handler(int sig) // signal handler function
    {
        switch(sig){
            case SIGHUP:
                // rehash the server
                syslog(LOG_ERR, "SIGHUP");
                break;
            case SIGTERM:
                //finalize the server
                syslog(LOG_ERR, "SIGTERM");
                exit(0);
                break;
        }
    }*/
QString *RutadatosVariables=NULL;
QString *Rutacertificadoclave=NULL;
int puerto=-1;
Servidor *server;
void Signal_Handler(int sig) /* signal handler function */
 {
     switch(sig){
         case SIGHUP:
             /* rehash the server */
             server->OpcionesLimpieza();
             delete server;
             server=new Servidor(*RutadatosVariables,*Rutacertificadoclave, puerto);
             server->inicioServer();
             syslog(LOG_ERR, "SIGHUP");
         break;
         case SIGTERM:
             /* finalize the server */
             syslog(LOG_ERR, "SIGTERM");
             exit(0);
         break;
     }
 }
int main(int argc, char *argv[])
{
    openlog(argv[0], LOG_NOWAIT | LOG_PID, LOG_USER);

    pid_t pid;
    // Nos clonamos a nosotros mismos creando un proceso hijo
    pid = fork();
    // Si pid es < 0, fork() falló
    if (pid < 0) {
        // Mostrar la descripción del error y terminar
        qDebug() << "PREFIERO MORIR DE PIE A DEMONIZARME DE RODILLAS";
        std::cerr << strerror(errno) << std::endl;
        exit(10);
    }
    // Si pid es > 0, estamos en el proceso padre
    if (pid > 0) {

        // Terminar el proceso
        exit(0);
    }
    //HIJO
    umask(0);
    close(STDIN_FILENO); // fd 0
    close(STDOUT_FILENO); // fd 1
    close(STDERR_FILENO); // fd 2

    int fd0 = open("/dev/null", O_RDONLY); // fd0 == 0
    int fd1 = open("/dev/null", O_WRONLY); // fd0 == 1
    int fd2 = open("/dev/null", O_WRONLY); // fd0 == 2

    QDir directorio;
    if(directorio.exists(QString(APP_CONFFILE))){
        QFile file(QString(APP_CONFFILE));
        if (file.open(QIODevice::ReadOnly)){
            QTextStream configuracion(&file);
            RutadatosVariables= new QString(configuracion.readLine());
            Rutacertificadoclave= new QString(configuracion.readLine());
            puerto=(configuracion.readLine()).toInt();
        }else
            syslog(LOG_ERR, "Archivo de configuracion no se pudo abrir");
        file.close();
    }else{
        syslog(LOG_ERR, "No hay archivo de configuracion");

    }

    if (RutadatosVariables==NULL)
        RutadatosVariables=new QString(APP_VARDIR);
    if (Rutacertificadoclave==NULL)
        Rutacertificadoclave=new QString(APP_CERTCLA);
    if (puerto==-1)
        puerto=33333;
    directorio.mkpath(*RutadatosVariables);
    directorio.mkpath(*Rutacertificadoclave);
    pid_t sid;
    // Intentar crear una nueva sesión
    sid = setsid();
    if (sid < 0) {
        syslog(LOG_ERR, "No fue posible crear una nueva sesiónn");
        exit(11);
    }

    if ((chdir("/")) < 0) {
        syslog(LOG_ERR, "No fue posible cambiar el directorio de "
                        "trabajo a /n");
        exit(12);
    }



    // Cambiar el grupo efectivo del proceso a 'midemonio'
    group* group = getgrnam("midemonio");

    //syslog(LOG_ERR, "GETGRNAM?");
    if(group!=NULL){
        if(setegid(group->gr_gid)!=0){
            syslog(LOG_ERR, "ERROR EN SETEGID");
            //syslog(LOG_ERR, strerror(errno));
        }
    }
    else
        syslog(LOG_ERR, "No existe el grupo");

    passwd* user = getpwnam("midemonio");
    const char* myChar =RutadatosVariables->toStdString().c_str();
    chown(myChar, user->pw_uid,group->gr_gid);
    myChar =Rutacertificadoclave->toStdString().c_str();
    chown(myChar, user->pw_uid,group->gr_gid);
    // Cambiar el usuario efectivo del proceso a 'midemonio'
    if (user!=NULL){
        if(seteuid(user->pw_uid)!=0){
            syslog(LOG_ERR, "ERROR EN SETEUID");
            //syslog(LOG_ERR, strerror(errno));
        }
    }
    else
        syslog(LOG_ERR, "No existe el usuario");


    //===============================================================
    server=new Servidor(*RutadatosVariables,*Rutacertificadoclave, puerto);
    QCoreApplication::setSetuidAllowed(true);
    QCoreApplication a(argc, argv);
    server->inicioServer();
    signal(SIGHUP,Signal_Handler); /* hangup signal */
    signal(SIGTERM,Signal_Handler); /* software termination signal from kill */


    return a.exec();
}
