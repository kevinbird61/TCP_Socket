#include "clientusage.h"

ClientUsage::ClientUsage(QString server_name, int server_port, QObject *parent)
    : QTcpSocket(parent)
{
    this->abort();
    this->connectToHost(QHostAddress(server_name),(qintptr)server_port);
    // Check the connection
    qDebug() << "Connecting to server : " << server_name << " ; within port : " << server_port;
    while(!this->waitForConnected()){ /* wait for connection */ }
    qDebug() << "Connect to server successfully!";

    connect(this,SIGNAL(readyRead()),this,SLOT(readRequest()));
    connect(this,SIGNAL(disconnected()),this,SLOT(disconnect()));
}

void ClientUsage::readRequest()
{
    qDebug() << "Start reading...";
    if(this->canReadLine()){
        qDebug() << "Read from server source (multiple line)...";
        while(this->canReadLine()){
            qDebug() << this->readLine();
        }
        qDebug() << "End read.";
    }
    else{
        qDebug() << "Read from server source : " << this->readLine();
    }
    // Prepare write to server
    this->write("Ok ! I receive it!");
}

void ClientUsage::disconnect()
{
    qDebug() << "Disconnect!";
}
