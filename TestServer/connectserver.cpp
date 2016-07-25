#include "connectserver.h"

ConnectServer::ConnectServer(QObject *parent)
    : QTcpServer(parent)
{
    threadID = 0;
}

void ConnectServer::incomingConnection(qintptr socketDescriptor)
{
        TcpThread *thread = new TcpThread(socketDescriptor,threadID);
        connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
        connect(thread,SIGNAL(releaseID()),this,SLOT(controlThreadID()));
        thread->start();
        threadID++;
}

void ConnectServer::controlThreadID()
{
    qDebug() << "Thread minus one";
    threadID -= 1;
}

