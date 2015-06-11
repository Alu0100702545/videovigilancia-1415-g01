#include "clientev.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_LOGGING_RULES", "qt.network.ssl.warning=false");
    QApplication a(argc, argv);
    ClienteV w;
    w.show();

    return a.exec();
}
