#include <QCoreApplication>
#include <QTimer>
#include "Client.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Create the client and connect to the server
    Client client;
    const QString host = "127.0.0.1"; // The server address
    const quint16 port = 5000; // The server port
    client.connectToServer(host, port);

    // Send a test message from the client to the server after a short delay
    QTimer::singleShot(1000, [&client]() {
        client.sendMessage("Hello, Server! This is a test message from the Client.");
    });

    return app.exec();
}
