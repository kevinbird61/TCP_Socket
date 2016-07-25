#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QStringList>
#include <QThread>
#include <QRegExp>
#include <QDir>
#include <iostream>

using namespace std;

class TcpThread : public QThread
{
    Q_OBJECT
public:
    TcpThread(qintptr socketDescriptor , int id);
    // For shell implement
    void readCommand(QString cmd);

signals:
    void releaseID();

public slots:
    void readFromClient();
    void disconnect();

private:
    QTcpSocket *client;
};

#endif // TCPTHREAD_H
