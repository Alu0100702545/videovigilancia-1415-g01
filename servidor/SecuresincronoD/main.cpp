
#include <QDebug>
#include <QCoreApplication>
#include "servidorvsincrono.h"
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

void Signal_Handler(int sig) /* signal handler function */
    {
        switch(sig){
            case SIGHUP:
                /* rehash the server */
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
        qDebug() << "KILL THE FATHER";
        // Terminar el proceso
        exit(0);
    }
    //HIJO
    umask(0);
    close(STDIN_FILENO); // fd 0
    close(STDOUT_FILENO); // fd 1
    close(STDERR_FILENO); // fd 2
    //syslog(LOG_ERR, "DESCRIPTORES CERRADOS!");
    int fd0 = open("/dev/null", O_RDONLY); // fd0 == 0
    int fd1 = open("/dev/null", O_WRONLY); // fd0 == 1
    int fd2 = open("/dev/null", O_WRONLY); // fd0 == 2
    //syslog(LOG_ERR, "PASADOS A FICHERO");
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

    // Cambiar el usuario efectivo del proceso a 'midemonio'

    // Cambiar el grupo efectivo del proceso a 'midemonio'
    group* group = getgrnam("midemonio");
    //syslog(LOG_ERR, "GETGRNAM?");
    if(group!=NULL){
        if(setegid(group->gr_gid)!=0){
            syslog(LOG_ERR, "ERROR EN SETEGID");
            syslog(LOG_ERR, strerror(errno));
        }
    }
    else
        syslog(LOG_ERR, "No existe el grupo");

    passwd* user = getpwnam("midemonio");
    syslog(LOG_ERR, "GETPWNAM?");
    if (user!=NULL){
        if(seteuid(user->pw_uid)!=0){
            syslog(LOG_ERR, "ERROR EN SETEUID");
            syslog(LOG_ERR, strerror(errno));
        }
    }
    else
        syslog(LOG_ERR, "No existe el usuario");

    syslog(LOG_ERR, "TUTU");
    //===============================================================
    QCoreApplication::setSetuidAllowed(true);
    QCoreApplication a(argc, argv);
    syslog(LOG_ERR, "BUCLE DE MENSAJES");
    servidorvsincrono server;
    //server.OpcionesLimpieza();
    server.inicioServer();
    signal(SIGHUP,Signal_Handler); /* hangup signal */
    signal(SIGTERM,Signal_Handler); /* software termination signal from kill */

    return a.exec();
}
