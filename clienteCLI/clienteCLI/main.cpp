#include <QCoreApplication>
#include <clientecli.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ClienteCLI w;
    w.debug();

    return a.exec();
}
