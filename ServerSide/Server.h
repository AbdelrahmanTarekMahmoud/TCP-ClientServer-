#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    bool startServer(quint16 port);
    void stopServer();
    void sendMessageToClient(QTcpSocket *client, const QString &message);
    void broadcastMessage(const QString &message);

private slots:
    void onNewConnection();
    void onClientDisconnected();
    void onReadyRead();

private:
    QTcpServer *tcpServer;
    QHash<QTcpSocket*, QString> clients;  // Store clients and their data

    void processMessage(QTcpSocket *client, const QString &message);
};

#endif // SERVER_H
