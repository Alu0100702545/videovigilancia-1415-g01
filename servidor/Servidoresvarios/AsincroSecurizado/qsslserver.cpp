#include "qsslserver.h"

QSslServer::QSslServer(QObject *parent, QSqlDatabase &Vdbs) : QTcpServer(parent),
Vdb(Vdbs)
{}

void QSslServer::incomingConnection(qintptr socketDescriptor) {
  QSslSocket *socket = new QSslSocket();
  if (socket->setSocketDescriptor(socketDescriptor)) {
       QFile fileCert("/home/fabix/Documentos/SOA/Proyecto_Videovigilancia/servidor/videovigilancia.crt");
    connect(socket,SIGNAL(sslErrors(QList<QSslError>)),socket,SLOT(ignoreSslErrors()));
    qDebug() <<"existe pem:"<<QFile::exists("/home/fabix/Documentos/SOA/Proyecto_Videovigilancia/servidor/videovigilancia.crt");
    qDebug() <<" ceritifado abierto: " <<fileCert.open(QIODevice::ReadOnly);
    fileCert.close();
    QFile algo("/home/fabix/Documentos/SOA/Proyecto_Videovigilancia/servidor/videovigilancia.key");
    qDebug() <<" Key abierta: " <<algo.open(QIODevice::ReadOnly);
    algo.close();
    socket->setProtocol(QSsl::AnyProtocol);
    socket->ignoreSslErrors();
    //socket->addDefaultCaCertificates("")
    socket->setPrivateKey("/home/fabix/Documentos/SOA/Proyecto_Videovigilancia/servidor/videovigilancia.key");
    socket->setPeerVerifyMode(QSslSocket::QueryPeer);
    socket->addCaCertificates("/home/fabix/Documentos/SOA/Proyecto_Videovigilancia/servidor/videovigilancia.crt");
    socket->setLocalCertificate("/home/fabix/Documentos/SOA/Proyecto_Videovigilancia/servidor/videovigilancia.crt");
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


