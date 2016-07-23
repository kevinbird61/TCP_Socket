#include "tcpthread.h"

TcpThread::TcpThread(qintptr socketDescriptor)
{
    // Initialize and connect
    client = new QTcpSocket(Q_NULLPTR);
    connect(client,SIGNAL(readyRead()),this,SLOT(readFromClient()));
    connect(client,SIGNAL(disconnected()),this,SLOT(disconnect()));
    // Prepare for client connection (Using thread to implement)
    qDebug() << "InComing Info Receive , prepare to create the dealer!!";
    // Check the client-side
    if(!client->setSocketDescriptor(socketDescriptor)){
        qDebug() << "Client fail within connection.";
    }
    qDebug() << "Client create successfully!";

    // Tell client , server is ready
    client->write("OK");
}

void TcpThread::readFromClient()
{
    QTcpSocket *client = (QTcpSocket*) sender();
    qDebug() << "Start Reading from client:";
    if(client->canReadLine()){
        qDebug() << "Read from client source (multiple line)...";
        while(client->canReadLine()){
            qDebug() << QString::fromUtf8(client->readLine());
        }
        qDebug() << "End read.";
    }
    else{
        qDebug() << "Read from client source : " << QString::fromUtf8(client->readLine());
    }

    // Read all , tell client , server is ready again
    client->write("OK");
}

void TcpThread::disconnect()
{
    qDebug() << "Disconnect by user.";
}
