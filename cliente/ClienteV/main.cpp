#include "clientev.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClienteV w;
    w.show();

    return a.exec();
}
