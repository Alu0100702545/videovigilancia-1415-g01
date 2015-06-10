#include "qsslserver.h"

QSslServer::QSslServer(QObject *parent, QSqlDatabase &Vdbs, QString RutadatosVariables,QString Rutacertificadoclave) : QTcpServer(parent),
Vdb(Vdbs)
{
    Rutadatos=RutadatosVariables;
    Rutacert=Rutacertificadoclave;

}

void QSslServer::incomingConnection(qintptr socketDescriptor) {
  QSslSocket *socket = new QSslSocket();
  if (socket->setSocketDescriptor(socketDescriptor)) {

    connect(socket ,SIGNAL(sslErrors(QList<QSslError>)),socket ,SLOT(ignoreSslErrors()));
    socket->setProtocol(QSsl::AnyProtocol);
    socket->ignoreSslErrors();
    socket->setPrivateKey(Rutacert+"/"+"videovigilancia.key");
    socket->setPeerVerifyMode(QSslSocket::QueryPeer);
    socket->addCaCertificates(Rutacert+"/"+"videovigilancia.crt");
    socket->setLocalCertificate(Rutacert+"/"+"videovigilancia.crt");
    socket->startServerEncryption();

    client *nuevaC =new client(socket,Vdb,Rutadatos,this);

  } else {
    delete socket;
  }


}


