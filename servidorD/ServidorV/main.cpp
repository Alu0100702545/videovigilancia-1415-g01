#include <QCoreApplication>
#include "servidor.h"
#include <QRegExp>
int main(int argc, char *argv[])
{
    QxtBasicFileLoggerEngine *debug =
    new QxtBasicFileLoggerEngine("debug.log");

    QxtBasicFileLoggerEngine *info =
    new QxtBasicFileLoggerEngine("info.log");

    // Cada engine se registra en el objeto QtLogger asignándole
    // un nombre diferente
    qxtLog->addLoggerEngine("dbg", debug);
    qxtLog->addLoggerEngine("app", info);

    // Finalmente se indica que niveles de severidad se envían a cada
    // engine y, por tanto, a cada archivo de registro
    qxtLog->disableAllLogLevels();
    qxtLog->enableLogLevels("dbg", QxtLogger::AllLevels);
    qxtLog->enableLogLevels("app", QxtLogger::InfoLevel |
    QxtLogger::WarningLevel |
    QxtLogger::ErrorLevel |
    QxtLogger::CriticalLevel |
    QxtLogger::FatalLevel |
    QxtLogger::WriteLevel );

    pid_t pid;
    // Nos clonamos a nosotros mismos creando un proceso hijo
    pid = fork();
    // Si pid es < 0, fork() falló
    if (pid < 0) {
        // Mostrar la descripción del error y terminar
        std::cerr << strerror(errono) << std::endl;
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
    passwd* user = getpwnam("midemonio");
    if (user!=NULL)
        seteuid(user->pw_uid);
    else
        qxtLog->info("No existe el usuario");

    // Cambiar el grupo efectivo del proceso a 'midemonio'
    group* group = getgrnam("midemonio");
    if(group!=NULL)
        setegid(group->gr_gid);
    else
        qxtLog->info("No existe el grupo");


    QCoreApplication a(argc, argv);
    Servidor server;
    server.inicioServer();
    server.OpcionesLimpieza();

    return a.exec();
}
