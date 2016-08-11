#include <QCoreApplication>
#include <clientusage.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // OPTION : server name and port can be modify by user(stdin)

    // Making TCP connection
    ClientUsage *clientSide = new ClientUsage("127.0.0.1",51617);

    return a.exec();
}
