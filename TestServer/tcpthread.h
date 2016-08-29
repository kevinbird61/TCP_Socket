#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QTextCodec>
#include <QStringList>
#include <QThread>
#include <QRegExp>
#include <QDir>
#include <iostream>
#include <fstream>

// Redirect to the server's storage
#define SERVER_FILE "available_download/"

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
    QTcpSocket *read_client;
    QByteArray file_storage;
    QByteArray temp_storage;
    QString current_filename;
    QString help_comment;
    int file_tag;
};

#endif // TCPTHREAD_H
