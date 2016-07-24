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

void TcpThread::readCommand(QString cmd)
{
    if(cmd == QString("quit")){
        qDebug() << "Client want to disconnect!";
        client->write("quit_OK");
    }
    else if(cmd == QString("help")){
        QString help_cmd = QString(
                    "request_help\n"
                    "help : show the command we support\n"
                    "quit : disconnect from server\n");
        client->write(help_cmd.toUtf8());
    }
    else{
        qDebug() << cmd << ". Not match!";
        // Read all , tell client , server is ready again
        client->write("request_help");
    }
}

void TcpThread::readFromClient()
{
    QTcpSocket *client = (QTcpSocket*) sender();
    qDebug() << "Start Reading from client:";
    if(client->canReadLine()){
        // Can implement in ChatRoom example , but now I don't need this
        /*qDebug() << "Read from client source (multiple line)...";
        while(client->canReadLine()){
            qDebug() << QString::fromUtf8(client->readLine());
        }
        qDebug() << "End read.";
        */
    }
    else{
        QString str = QString::fromUtf8(client->readLine());
        qDebug() << "Read from client source : " << str;
        // identify the command send from client side
        readCommand(str);
    }
}

void TcpThread::disconnect()
{
    qDebug() << "Disconnect by user.";
}
