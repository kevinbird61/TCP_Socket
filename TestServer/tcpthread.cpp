#include "tcpthread.h"

TcpThread::TcpThread(qintptr socketDescriptor, int id)
{
    // Initialize and connect
    client = new QTcpSocket(Q_NULLPTR);
    connect(client,SIGNAL(readyRead()),this,SLOT(readFromClient()));
    connect(client,SIGNAL(disconnected()),this,SLOT(disconnect()));
    // Prepare for client connection (Using thread to implement)
    qDebug() << "Using TcpThread : " << id;
    qDebug() << "InComing Info Receive , prepare to create the dealer!!";
    // Check the client-side
    if(!client->setSocketDescriptor(socketDescriptor)){
        qDebug() << "Client fail within connection.";
    }
    qDebug() << "Client create successfully!";
    qDebug() << "Client side info , Address : " << client->peerAddress().toString() << " ; Port : " << client->peerPort();
    // Tell client , server is ready
    client->write("OK");
}

void TcpThread::readCommand(QString cmd)
{
    // ========================== Pre-work ==========================
    QRegExp download("download_*");
    download.setPatternSyntax(QRegExp::Wildcard);
    // ========================== Pre-work ==========================

    if(cmd.compare(QString("quit"),Qt::CaseSensitive) == 0){
        qDebug() << "Client want to disconnect!";
        client->write("quit_OK");
    }
    else if(cmd.compare(QString("help"),Qt::CaseSensitive) == 0){
        QString help_cmd = QString(
                    "request_help\n"
                    "help : show the command we support\n"
                    "quit : disconnect from server\n"
                    "ls : show the file in server side (can download)\n"
                    "download_filename : download option , which can download the file from server side with the filename"
                    );
        client->write(help_cmd.toUtf8());
    }
    else if(cmd.compare(QString("ls"),Qt::CaseSensitive) == 0){
        QDir dir;
        dir.setFilter(QDir::Files | QDir::Hidden );
        QStringList *list = new QStringList(dir.entryList());
        QString listfile = QString("request_ls\n");
        for(int i = 0 ; i < list->size() ; i ++){
                listfile += list->at(i) + "\n";
        }
        client->write(listfile.toUtf8());
    }
    else if(download.exactMatch(cmd)){ // using regular expression to match
        // Get the filename that user want to download
        QString match = cmd.section("_",1,1);
        if(!match.isEmpty())
            qDebug() << "Compare Match!! Result : " << match ;
        else{
            qDebug() << "Compare failure!";
            client->write(QString("request_download_fail").toUtf8());
            return;
        }
        // Check the file is exist or not
        QFile download_File(match);
        if(!download_File.exists()){
            qDebug() << "This file doesn't available!";
            client->write(QString("request_download_fail").toUtf8());
            return;
        }
        // If available , using data stream to deliver
        QString d_result = QString("request_download_"+match+"\n");
        download_File.open(QIODevice::ReadOnly);
        QDataStream writein(&download_File);
        char *test = new char[download_File.size()];
        writein.readRawData(test,download_File.size());
        qDebug() << test;
        d_result += QString(test);
        download_File.close();
        // Send back the message and file content to client.
        client->write(d_result.toUtf8());
    }
    else{
        qDebug() << cmd << ". Not match!";
        // Read all , tell client , server is ready again
        client->write("request_help");
    }
}

void TcpThread::readFromClient()
{
    QTcpSocket *client = (QTcpSocket*) sender();
    qDebug() << "Start Reading from client:";
    if(client->canReadLine()){
        // Can implement in ChatRoom example , but now I don't need this
        /*qDebug() << "Read from client source (multiple line)...";
        while(client->canReadLine()){
            qDebug() << QString::fromUtf8(client->readLine());
        }
        qDebug() << "End read.";
        */
    }
    else{
        QString str = QString::fromUtf8(client->readLine());
        qDebug() << "Read from client source : " << str;
        // identify the command send from client side
        readCommand(str);
    }
}

void TcpThread::disconnect()
{
    qDebug() << "Disconnect by user.";
    emit releaseID();
}
