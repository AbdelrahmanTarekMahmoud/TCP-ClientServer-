#include <QCoreApplication>
#include "Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Create and start the server
    Server server;
    const quint16 port = 5000; // Port to run the server on
    if (server.startServer(port)) {
        qDebug() << "Server is running on port" << port;
    } else {
        qDebug() << "Failed to start server.";
        return -1; // Exit the application if the server fails to start
    }

    return app.exec();
}
