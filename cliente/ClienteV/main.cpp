#include "uicliente.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    uicliente w;
    w.show();

    return a.exec();
}
