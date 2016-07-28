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
        QDir dir(QDir::currentPath() + "/" + SERVER_FILE);
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
        QFile download_File(SERVER_FILE+match);
        if(!download_File.exists()){
            qDebug() << "This file doesn't available!";
            client->write(QString("request_download_fail").toUtf8());
            return;
        }
        // If available , using data stream to deliver
        QString f_src,f_content,d_result = QString("request_download_"+match+"\n");
        download_File.open(QIODevice::ReadOnly);

        QStringList f_name_content = d_result.split(".");
        f_content = f_name_content.at(1);
        f_content.chop(1);
        qDebug() << f_content;

        if(f_content == "bin" || f_content == "exe" || f_content == "dat"){
            // using binary read , without any toUnicode
            f_src = readFileSource(SERVER_FILE+match , 0);
        }
        else if( f_content == "txt" || f_content == "doc" || f_content == "csv"){
            f_src = readFileSource(SERVER_FILE+match , 1);
        }
        else{
            f_src = readFileSource(SERVER_FILE+match , 0);
        }
        qDebug() << f_src;
        d_result += f_src;
        download_File.close();
        // Send back the message and file content to client. Tell client ready to get the multi-times of send and receive work if the file is huge.
        client->write(d_result.toUtf8());
    }
    else{
        qDebug() << cmd << ". Not match!";
        // Read all , tell client , server is ready again
        QString help_cmd = QString(
                    "request_help\n"
                    "help : show the command we support\n"
                    "quit : disconnect from server\n"
                    "ls : show the file in server side (can download)\n"
                    "download_filename : download option , which can download the file from server side with the filename"
                    );
        client->write(help_cmd.toUtf8());
    }
}

QString TcpThread::readFileSource(QString filepath , int mode)
{
    string str,result;
    ifstream myFile(filepath.toStdString() , ios::in | ios::binary);
    QTextCodec *codec = QTextCodec::codecForName("Big5");
    while(getline(myFile,str)){
        qDebug() << QString::fromStdString(str);
        // Decode whether if there are chinese
        if(mode == 0){
            result += str;
        }
        else{
            result += codec->toUnicode(str.c_str()).toStdString() + "\n";
        }
    }
    myFile.close();
    return QString::fromStdString(result.c_str());
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
