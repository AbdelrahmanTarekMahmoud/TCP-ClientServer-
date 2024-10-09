#include "Client.h"
#include <QDebug>

Client::Client(QObject *parent) : QObject(parent), tcpSocket(new QTcpSocket(this))
{
    connect(tcpSocket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &Client::onErrorOccurred);
}

Client::~Client()
{
    disconnectFromServer();
}

void Client::connectToServer(const QString &host, quint16 port)
{
    qDebug() << "Connecting to server at" << host << ":" << port;
    tcpSocket->connectToHost(host, port);
}

void Client::disconnectFromServer()
{
    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        tcpSocket->disconnectFromHost();
    }
}

void Client::sendMessage(const QString &message)
{
    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        tcpSocket->write(message.toUtf8());
    }
}

void Client::onConnected()
{
    qDebug() << "Connected to server.";
}

void Client::onDisconnected()
{
    qDebug() << "Disconnected from server.";
}

void Client::onReadyRead()
{
    QString message = tcpSocket->readAll();
    qDebug() << "Message received from server:" << message;
}

void Client::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error:" << tcpSocket->errorString();
}
