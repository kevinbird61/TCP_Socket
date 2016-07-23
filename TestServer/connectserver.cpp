#include "connectserver.h"

ConnectServer::ConnectServer(QObject *parent)
    : QTcpServer(parent)
{
    client = new QTcpSocket(Q_NULLPTR);
    connect(client,SIGNAL(readyRead()),this,SLOT(readFromClient()));
    connect(client,SIGNAL(disconnected()),this,SLOT(disconnect()));
}

void ConnectServer::incomingConnection(qintptr socketDescriptor)
{
    // Prepare for client connection (Using thread to implement)
    qDebug() << "InComing Info Receive , prepare to create the dealer!!";
    // Check the client-side
    if(!client->setSocketDescriptor(socketDescriptor)){
        qDebug() << "Client fail within connection.";
    }
    qDebug() << "Client create successfully!";

    client->write("Hello Kevin!");
}

void ConnectServer::readFromClient()
{
    QTcpSocket *client = (QTcpSocket*) sender();
    qDebug() << "Start Reading from client:";
    if(client->canReadLine()){
        qDebug() << "Read from client source (multiple line)...";
        while(client->canReadLine()){
            qDebug() << client->readLine();
        }
        qDebug() << "End read.";
    }
    else{
        qDebug() << "Read from client source : " << client->readLine();
    }

}

void ConnectServer::disconnect()
{
    qDebug() << "Disconnect!";
    QTcpSocket *client = (QTcpSocket*)sender();
    client->write("Thank for your usage!");
}
