#include "clientusage.h"

ClientUsage::ClientUsage(QString server_name, int server_port, QObject *parent)
    : QTcpSocket(parent)
{
    this->abort();
    this->connectToHost(QHostAddress(server_name),(qintptr)server_port);
    // Check the connection
    qDebug() << "Connecting to server : " << server_name << " ; within port : " << server_port;
    while(!this->waitForConnected()){ /* wait for connection */ }
    qDebug() << "Connect to server successfully!";

    connect(this,SIGNAL(readyRead()),this,SLOT(readRequest()));
    connect(this,SIGNAL(disconnected()),this,SLOT(disconnect()));
}

void ClientUsage::readRequest()
{
    // ========================== Pre-work ==========================
    QRegExp check_download("request_download_*");
    check_download.setPatternSyntax(QRegExp::Wildcard);
    // ========================== Pre-work ==========================
    qDebug() << "Start reading...";
    // Fetch the comment from server
    QString str = this->readLine().trimmed();
    // Server's reply depend on user command , so complex command should have multiple send&receive!
    if(str == "OK"){
        // You can send command now!
        qDebug() << "*****You can send your command now!*****";
        cout << "LiveGamer@Client_Socket$:";
        char cmdin[128];
        cin >> cmdin;
        this->write(QString(cmdin).toUtf8());
    }
    else if(str == "request_ls"){
        // Print out the file within server side , which is available
        qDebug() << "*****The files which are available in server side.*****";
        while(this->canReadLine()){
            cout << this->readLine().toStdString();
        }
        qDebug() << "*****You can using download command to fetch them to client side.*****";
        qDebug() << "*****You can send your command now!*****";
        cout << "LiveGamer@Client_Socket$:";
        char cmdin[128];
        cin >> cmdin;
        this->write(QString(cmdin).toUtf8());
    }
    else if(str == "request_help"){
        // Print out all existing command , which send from server
        qDebug() << "*****The command we support : *****";
        while(this->canReadLine()){
            cout << this->readLine().toStdString();
        }
        qDebug() << "*****Thank for your usage and please retype again.*****";
        qDebug() << "*****You can send your command now!*****";
        cout << "LiveGamer@Client_Socket$:";
        char cmdin[128];
        cin >> cmdin;
        this->write(QString(cmdin).toUtf8());
    }
    else if(check_download.exactMatch(str)){
        // In download receive , check if it is available or not
        QString check = str.section("_",2,2);
        if(check.compare(QString("fail"),Qt::CaseSensitive) == 0){
            qDebug() << "*****Download failure !*****";
            qDebug() << "*****You can send your command now!*****";
            cout << "LiveGamer@Client_Socket$:";
            char cmdin[128];
            cin >> cmdin;
            this->write(QString(cmdin).toUtf8());
        }
        else{
            // We can store this file with name $check's value
            QFile result_file("DownloadTest/"+check);
            result_file.open(QFile::WriteOnly);
            qDebug() << "Storing....";
            QString result = this->readAll();
            qDebug() << result.toUtf8();
            result_file.write(result.toLocal8Bit().data(),qstrlen(result.toLocal8Bit().data()));
            result_file.close();
            qDebug() << "*****Storage Complete!*****";
            qDebug() << "*****You can send your command now!*****";
            cout << "LiveGamer@Client_Socket$:";
            char cmdin[128];
            cin >> cmdin;
            this->write(QString(cmdin).toUtf8());
        }
    }
    else if(str == "quit_OK"){
        this->abort();
    }
    else{
        qDebug() << "*****You can't send it , server hasn't prepared!*****";
    }
}

void ClientUsage::disconnect()
{
    qDebug() << "Disconnect!";
}
