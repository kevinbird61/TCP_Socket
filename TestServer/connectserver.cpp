#include "connectserver.h"

ConnectServer::ConnectServer(QObject *parent)
    : QTcpServer(parent)
{

}

void ConnectServer::incomingConnection(qintptr socketDescriptor)
{
        TcpThread *thread = new TcpThread(socketDescriptor);
        connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
        thread->start();
}

