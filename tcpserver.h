#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QString>

class clientSocket:public QTcpSocket
{
    Q_OBJECT
public:
    clientSocket(QObject *parent=nullptr);
private:
    QTimer *connectTimer;
};

class tcpServer:public QTcpServer
{
    Q_OBJECT

    const int dataSize=20;

public:
    tcpServer(QObject *parent=nullptr);

    enum deviceName
    {
        TouchScreen=0,
        ControlModule,
        deviceNum
    };

signals:
    deviceConnected(const QString &);
    deviceDisconnected(const QString &);
    hostMessage(const std::string &);
private slots:
    void dealTouchScreenMessage();
    void dealControlModuleMessage();
private:
    void incomingConnection(qintptr handle);

    void sendMessage(const char *message,qint64 len,QTcpSocket *goalSocket);

    QString touchScreenAddr=QString("::ffff:192.168.1.101");
    QString controlModuleAddr=QString("::ffff:192.168.1.102");

    clientSocket *touchScreenSocket=nullptr;
    clientSocket *controlModuleSocket=nullptr;
};

#endif // TCPSERVER_H
