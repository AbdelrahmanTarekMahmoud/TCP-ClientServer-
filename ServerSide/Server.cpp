#include "Server.h"
#include <QDebug>

Server::Server(QObject *parent) : QObject(parent), tcpServer(new QTcpServer(this))
{
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

Server::~Server()
{
    stopServer();
}

bool Server::startServer(quint16 port)
{
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Server failed to start: " << tcpServer->errorString();
        return false;
    }
    qDebug() << "Server started on port:" << port;
    return true;
}

void Server::stopServer()
{
    foreach(QTcpSocket *client, clients.keys()) {
        client->disconnectFromHost();
        client->deleteLater();
    }
    tcpServer->close();
    qDebug() << "Server stopped.";
}

void Server::onNewConnection()
{
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    clients[clientSocket] = clientSocket->peerAddress().toString();
    qDebug() << "New client connected from:" << clients[clientSocket];

    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
}

void Server::onClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        qDebug() << "Client disconnected:" << clients[clientSocket];
        clients.remove(clientSocket);
        clientSocket->deleteLater();
    }
}

void Server::onReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        QString message = clientSocket->readAll();
        qDebug() << "Message received from" << clients[clientSocket] << ":" << message;
        processMessage(clientSocket, message);
    }
}

void Server::sendMessageToClient(QTcpSocket *client, const QString &message)
{
    if (client && client->state() == QAbstractSocket::ConnectedState) {
        client->write(message.toUtf8());
    }
}

void Server::broadcastMessage(const QString &message)
{
    foreach(QTcpSocket *client, clients.keys()) {
        sendMessageToClient(client, message);
    }
}

void Server::processMessage(QTcpSocket *client, const QString &message)
{
    // Handle incoming messages from clients here
    qDebug() << "Processing message:" << message;
}
