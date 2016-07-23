#ifndef CONNECTSERVER_H
#define CONNECTSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class ConnectServer : public QTcpServer
{
    Q_OBJECT
public:
    ConnectServer(QObject *parent=0);
    void incomingConnection(qintptr socketDescriptor);

public slots:
    void readFromClient();
    void disconnect();

private:
    QTcpSocket *client;
};

#endif // CONNECTSERVER_H
