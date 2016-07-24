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
    // Server's reply depend on user command , so complex command should have multiple send&receive!
    if(str == "OK"){
        // You can send command now!
        qDebug() << "You can send your command now!";
        char cmdin[128];
        cin >> cmdin;
        this->write(QString(cmdin).toUtf8());
    }
    else if(str == "request_help"){
        // Print out all existing command , which send from server
        qDebug() << "The command we support : ";
        while(this->canReadLine()){
            cout << this->readLine().toStdString();
        }
        qDebug() << "Thank for your usage and please retype again.";
        char cmdin[128];
        cin >> cmdin;
        this->write(QString(cmdin).toUtf8());
    }
    else if(str == "quit_OK"){
        this->abort();
    }
    else{
        qDebug() << "You can't send it , server hasn't prepared!";
    }
}

void ClientUsage::disconnect()
{
    qDebug() << "Disconnect!";
}
