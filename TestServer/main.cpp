#include <QCoreApplication>
#include <QHostAddress>
#include <QDebug>
#include <connectserver.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // Create Server for listening
    ConnectServer *server = new ConnectServer();
    if(!server->listen(QHostAddress::Any,51617)){
        qDebug() << "server can't listen connection!";
        return 0;
    }
    qDebug() << "Success listening!";

    return a.exec();
}
