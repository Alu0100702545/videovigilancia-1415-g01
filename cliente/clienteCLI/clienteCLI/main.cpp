#include <QCoreApplication>
#include <QCommandLineParser>
#include <clientecli.h>
#include <QDebug>
#include <signal.h>

ClienteCLI* w;

void signalHandler(int signum)
{
    w->termSignalHandler(signum);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Cliente formato CLI");
    parser.addHelpOption();

    // An option with a value
    QCommandLineOption CamerasOption(QStringList() << "c" << "cameras",
            QCoreApplication::translate("main", "Indica los indices de las camaras"),
                                     QCoreApplication::translate("main", "indices"));
    parser.addOption(CamerasOption);

    // An option with a value
    QCommandLineOption IPOption(QStringList() << "i" << "ip",
            QCoreApplication::translate("main", "Especifica IP"),
            QCoreApplication::translate("main", "IP"));
    parser.addOption(IPOption);

    // An option with a value
    QCommandLineOption PortOption(QStringList() << "p" << "port",
            QCoreApplication::translate("main", "Especifica el puerto"),
            QCoreApplication::translate("main", "Puerto"));
    parser.addOption(PortOption);

    // Process the actual command line arguments given by the user
    parser.process(a);

    //======================================================================

    QRegExp rx("(\\,|\\/|\\:|\\t)");
    QStringList query = parser.value(CamerasOption).split(rx);

    w = new ClienteCLI;
    w->actualizar();

    if(parser.isSet(CamerasOption)){
        for(int i=0; i<query.size(); i++){
            qDebug() << "INDICE " << i << ": " << query.at(i).toInt();
            w->actualizar_cam((query.at(i)).toInt(),true);
        }
    }

    if(parser.isSet(IPOption)){
        qDebug() << "IP: " << parser.value(IPOption);
        w->actualizar_IP(parser.value(IPOption));
    }

    if(parser.isSet(PortOption)){
        qDebug() << "Puerto: " << parser.value(PortOption).toInt();
        w->actualizar_puerto(parser.value(PortOption).toInt());
    }

    w->capturar();

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    return a.exec();
}
