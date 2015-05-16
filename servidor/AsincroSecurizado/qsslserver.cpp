#include "qsslserver.h"

QSslServer::QSslServer(QObject *parent, QSqlDatabase &Vdbs) : QTcpServer(parent),
Vdb(Vdbs)
{}

void QSslServer::incomingConnection(qintptr socketDescriptor) {
  QSslSocket *socket = new QSslSocket();
  if (socket->setSocketDescriptor(socketDescriptor)) {
       QFile fileCert("videovigilancia.crt");
    connect(socket,SIGNAL(sslErrors(QList<QSslError>)),socket,SLOT(ignoreSslErrors()));
    qDebug() <<"existe pem:"<<QFile::exists("videovigilancia.crt");
    qDebug() <<" ceritifado abierto: " <<fileCert.open(QIODevice::ReadOnly);
    fileCert.close();
    QFile algo("videovigilancia.key");
    qDebug() <<" Key abierta: " <<algo.open(QIODevice::ReadOnly);
    algo.close();
    socket->setProtocol(QSsl::AnyProtocol);
    socket->ignoreSslErrors();
    //socket->addDefaultCaCertificates("")
    socket->setPrivateKey("videovigilancia.key");
    socket->setPeerVerifyMode(QSslSocket::QueryPeer);
    socket->addCaCertificates("videovigilancia.crt");
    socket->setLocalCertificate("videovigilancia.crt");
    socket->startServerEncryption();
    qDebug() <<"certificado es nulo ?: "<<socket->localCertificate().isNull();
    client *nuevaC =new client(socket,Vdb,this);

    qDebug() <<"soy valido ?: "<<socket->isValid();
    qDebug() <<"estoy encriptado ?: "<<socket->isEncrypted();
    qDebug() << "nuevaConexion";
  } else {
    delete socket;
  }


}


