#ifndef CLIENTUSAGE_H
#define CLIENTUSAGE_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QTextCodec>
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <iostream>

using namespace std;

class ClientUsage : public QTcpSocket
{
    Q_OBJECT
public:
    ClientUsage(QString server_name, int server_port, QObject *parent=0);

public slots:
    void readRequest();
    void disconnect();

private:
    int file_tag;
    QString current_file_name ;
    QByteArray temp_storage;
};

#endif // CLIENTUSAGE_H
