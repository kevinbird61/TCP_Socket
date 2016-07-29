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
    /* Using regular expression to test and match */
    QRegExp download("download_*");
    download.setPatternSyntax(QRegExp::Wildcard);
    /* When the file is big enough , need to separate and then can send it. */
    QRegExp conti_dl;
    if(!current_filename.isEmpty()){
        conti_dl = QRegExp(current_filename+"_*");
        conti_dl.setPatternSyntax(QRegExp::Wildcard);
    }
    // ========================== Pre-work ==========================

    /* quit command */
    if(cmd.compare(QString("quit"),Qt::CaseSensitive) == 0){
        qDebug() << "Client want to disconnect!";
        client->write("quit_OK");
    }
    /* help command */
    else if(cmd.compare(QString("help"),Qt::CaseSensitive) == 0){
        QString help_cmd = QString(
                    "request_help\n"
                    "help : show the command we support\n"
                    "quit : disconnect from server\n"
                    "ls : show the file in server side (can download)\n"
                    "download_filename : download option , which can download the file from server side with the filename\n"
                    );
        client->write(help_cmd.toUtf8());
    }
    /* ls command */
    else if(cmd.compare(QString("ls"),Qt::CaseSensitive) == 0){
        QDir dir(QDir::currentPath() + "/" + SERVER_FILE);
        dir.setFilter(QDir::Files | QDir::Hidden );
        QStringList *list = new QStringList(dir.entryList());
        QString listfile = QString("request_ls\n");
        for(int i = 0 ; i < list->size() ; i ++){
                listfile += list->at(i) + "\n";
        }
        client->write(listfile.toUtf8());
    }
    /* download start */
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
        QFile download_File(SERVER_FILE+match);
        if(!download_File.exists()){
            qDebug() << "This file doesn't available!";
            client->write(QString("request_download_fail").toUtf8());
            return;
        }
        // If available , using data stream to deliver
        QString d_result = QString("request_download_"+match+"\n");
        current_filename = match;
        download_File.open(QIODevice::ReadOnly);
        file_storage.clear();
        file_storage = download_File.readAll();
        download_File.close();
        // Send back the message and file content to client. Tell client ready to get the multi-times of send and receive work if the file is huge.
        client->write(QByteArray(d_result.toStdString().c_str(),d_result.size()));
    }
    /* download and continue send */
    else if(conti_dl.exactMatch(cmd)){
        // deal with file deliver
        QString check = cmd.section("_",1,1);
        // Get the current start position
        int current_tag = check.toInt();
        int capacity = 16384;
        int start = (current_tag-1)*capacity;
        int end = (current_tag)*capacity;
        // Base on check , as the current tag we has to send
        if(end > file_storage.length()){
            QByteArray send_piece("request_download_complete\n");
            send_piece += (file_storage.mid(start));
            client->write(send_piece);
        }
        else{
            QByteArray send_piece("request_download_conti\n");
            send_piece += (file_storage.mid( start , capacity));
            client->write(send_piece);
        }
    }
    /* Can't not recognize command */
    else{
        qDebug() << cmd << ". Not match!";
        // Read all , tell client , server is ready again
        QString notice_cmd = QString(
                    "request_help\n"
                    "help : show the command we support\n"
                    "quit : disconnect from server\n"
                    "ls : show the file in server side (can download)\n"
                    "download_filename : download option , which can download the file from server side with the filename\n"
                    );
        client->write(notice_cmd.toUtf8());
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
