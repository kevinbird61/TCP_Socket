<<<<<<< HEAD
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

=======
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

>>>>>>> c6b8f6f1ae4e85169dc24b71c3a66b653a48e009
