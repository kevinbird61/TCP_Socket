#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QTcpSocket>
#include <QThread>

class TcpThread : public QThread
{
    Q_OBJECT
public:
    TcpThread(qintptr socketDescriptor);
    // For shell implement
    void readCommand(QString cmd);

public slots:
    void readFromClient();
    void disconnect();

private:
    QTcpSocket *client;
};

#endif // TCPTHREAD_H
