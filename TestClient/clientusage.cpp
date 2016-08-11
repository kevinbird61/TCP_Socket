<<<<<<< HEAD
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
    QRegExp check_upload("request_upload_*");
    check_upload.setPatternSyntax(QRegExp::Wildcard);
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
        else if(check.compare(QString("conti"),Qt::CaseSensitive) == 0){
            // Continue file transmit
            QByteArray temp = this->readAll();
            temp_storage += temp;
            file_tag++;
            QString sender = current_file_name + "_" + QString::number(file_tag);
            this->flush();
            this->write(sender.toUtf8());
        }
        else if(check.compare(QString("complete"),Qt::CaseSensitive) == 0){
            // Success done all transmit and can store with the file.
            QByteArray temp = this->readAll();
            temp_storage += temp;
            // Write into file
            QFile result_file("DownloadTest/"+current_file_name);
            result_file.open(QFile::WriteOnly);
            result_file.write(temp_storage);
            result_file.close();
            qDebug() << "*****Download Successfully !*****";
            qDebug() << "*****You can send your command now!*****";
            cout << "LiveGamer@Client_Socket$:";
            char cmdin[128];
            cin >> cmdin;
            this->write(QString(cmdin).toUtf8());
        }
        else{
            // We can store this file with name $check's value
            // start the do the multiple send and receive work with file (when the file is getting bigger do) , and save the file name
            file_tag = 1;
            current_file_name = check;
            temp_storage = "";
            qDebug() << "*****Start to receive file from server! Please wait until to the whole file is transmitted.*****";
            check += "_" + QString::number(file_tag);
            this->flush();
            this->write(check.toUtf8());
        }
    }
    else if(check_upload.exactMatch(str)){
        // Server recognize with upload command
        QString file_name = str.section("_",2,2);
        QString file_no = str.section("_",3,3);
        int current_tag = file_no.toInt();
        if(current_tag == 1){
            // First package : find the file location , check if the file is exist or not
            QFile upload_File("upload/"+file_name);
            if(!upload_File.exists()){
                qDebug() << "This file doesn't exist in client side! Please check it again!";
                qDebug() << "*****You can send your command now!*****";
                cout << "LiveGamer@Client_Socket$:";
                char cmdin[128];
                cin >> cmdin;
                this->write(QString(cmdin).toUtf8());
                return;
            }
            // If available , load out all the data info from file
            upload_File.open(QIODevice::ReadOnly);
            temp_storage.clear();
            temp_storage = upload_File.readAll();
            upload_File.close();
        }
        int capacity = 16384;
        int start = (current_tag-1)*capacity;
        int end = (current_tag)*capacity;
        if(end > temp_storage.length()){
            QByteArray send_piece("upload_complete_none\n");
            send_piece += temp_storage.mid(start);
            this->write(send_piece);
        }
        else{
            QByteArray send_piece("upload_conti_none\n");
            send_piece += temp_storage.mid(start,capacity);
            this->write(send_piece);
        }
    }
    else if(str == "quit_OK"){
        this->abort();
    }
    else if(str == "request_jobdown"){
        qDebug() << "*****You can send your command now!*****";
        cout << "LiveGamer@Client_Socket$:";
        char cmdin[128];
        cin >> cmdin;
        this->write(QString(cmdin).toUtf8());
    }
    else{
        qDebug() << "Fail to match : " << str;
        qDebug() << "*****You can't send it , server hasn't prepared!*****";
    }
}

void ClientUsage::disconnect()
{
    qDebug() << "Disconnect!";
}
=======
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
        else if(check.compare(QString("conti"),Qt::CaseSensitive) == 0){
            // Continue file transmit
            QByteArray temp = this->readAll();
            temp_storage += temp;
            file_tag++;
            QString sender = current_file_name + "_" + QString::number(file_tag);
            this->flush();
            this->write(sender.toUtf8());
        }
        else if(check.compare(QString("complete"),Qt::CaseSensitive) == 0){
            // Success done all transmit and can store with the file.
            QByteArray temp = this->readAll();
            temp_storage += temp;
            // Write into file
            QFile result_file("DownloadTest/"+current_file_name);
            result_file.open(QFile::WriteOnly);
            result_file.write(temp_storage);
            result_file.close();
            qDebug() << "*****Download Successfully !*****";
            qDebug() << "*****You can send your command now!*****";
            cout << "LiveGamer@Client_Socket$:";
            char cmdin[128];
            cin >> cmdin;
            this->write(QString(cmdin).toUtf8());
        }
        else{
            // We can store this file with name $check's value
            // start the do the multiple send and receive work with file (when the file is getting bigger do) , and save the file name
            file_tag = 1;
            current_file_name = check;
            temp_storage = "";
            qDebug() << "*****Start to receive file from server! Please wait until to the whole file is transmitted.*****";
            check += "_" + QString::number(file_tag);
            this->flush();
            this->write(check.toUtf8());
        }
    }
    else if(str == "quit_OK"){
        this->abort();
    }
    else{
        qDebug() << "Fail to match : " << str;
        qDebug() << "*****You can't send it , server hasn't prepared!*****";
    }
}

void ClientUsage::disconnect()
{
    qDebug() << "Disconnect!";
}
>>>>>>> c6b8f6f1ae4e85169dc24b71c3a66b653a48e009
