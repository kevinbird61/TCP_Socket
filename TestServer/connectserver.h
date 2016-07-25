#ifndef CONNECTSERVER_H
#define CONNECTSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <tcpthread.h>

class ConnectServer : public QTcpServer
{
    Q_OBJECT
public:
    ConnectServer(QObject *parent=0);
    void incomingConnection(qintptr socketDescriptor);

public slots:
    void controlThreadID();

private:
    int threadID;
};

#endif // CONNECTSERVER_H
