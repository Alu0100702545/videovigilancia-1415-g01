#ifndef UICLIENTE_H
#define UICLIENTE_H

#include <QMainWindow>

namespace Ui {
class uicliente;
}

class uicliente : public QMainWindow
{
    Q_OBJECT

public:
    explicit uicliente(QWidget *parent = 0);
    ~uicliente();

private:
    Ui::uicliente *ui;
};

#endif // UICLIENTE_H
