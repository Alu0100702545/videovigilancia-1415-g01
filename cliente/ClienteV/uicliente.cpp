#include "uicliente.h"
#include "ui_uicliente.h"

uicliente::uicliente(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::uicliente)
{
    ui->setupUi(this);
}

uicliente::~uicliente()
{
    delete ui;
}
