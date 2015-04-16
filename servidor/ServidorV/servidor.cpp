#include "servidor.h"

void Servidor::deserializacion()
{


    QString aux;
    std::string aux2;
    if ((unsigned(client->bytesAvailable()) >= sizeof(qint32)) && (Tpaquete==0))
        Tpaquete=client->read(sizeof(qint32)).toInt();
    if (Tpaquete !=0 && client->bytesAvailable() >=Tpaquete ){
        aux=client->read(Tpaquete);
        aux2=aux.toStdString();
        paquete.ParseFromString(aux2);
        Tpaquete =0;
        emit algo();

    }
}

Servidor::Servidor()
{

}

void Servidor::almacenamiento()
{

}

