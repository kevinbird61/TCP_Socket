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
    // Fetch the comment from server
    QString str = this->readLine().trimmed();
    qDebug() << str << ".";
    if(str == "OK"){
        // You can send command now!
        char cmdin[128];
        cin >> cmdin;
        this->write(QString(cmdin).toUtf8());
    }
    else{
        qDebug() << "You can't send it , server hasn't prepared!";
    }
}

void ClientUsage::disconnect()
{
    qDebug() << "Disconnect!";
}
