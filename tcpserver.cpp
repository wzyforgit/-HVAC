#include "tcpserver.h"
#include <QDataStream>
#include <iostream>

clientSocket::clientSocket(QObject *parent):QTcpSocket(parent)
{
    /*connectTimer=new QTimer(this);
    connectTimer->start(1000);
    connect(connectTimer,&QTimer::timeout,[=](){
        write("connect");
    });*/

    connect(this,static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&clientSocket::error),[=](){
            qDebug()<<this->errorString()<<"\n";
    });
}

tcpServer::tcpServer(QObject *parent) : QTcpServer(parent)
{
}

void tcpServer::sendMessage(const char *message,qint64 len,QTcpSocket *goalSocket)
{
    goalSocket->write(message,len);
}

void tcpServer::dealTouchScreenMessage()
{
    int boxNum=touchScreenSocket->bytesAvailable()/20;
    auto message=touchScreenSocket->readAll();
    for(auto pMessage=message.data();boxNum!=0;--boxNum,pMessage+=20)
    {
        switch(pMessage[0])
        {
        default:break;
        case 'C'://数据转发到控制模块
            if(controlModuleSocket==nullptr)
            {
                sendMessage("1",1,touchScreenSocket);
                break;
            }
            else
            {
                sendMessage(pMessage,20,controlModuleSocket);
                sendMessage("0",1,touchScreenSocket);
                break;
            }
        }
    }
}

void tcpServer::dealControlModuleMessage()
{
    int boxNum=controlModuleSocket->bytesAvailable()/20;
    auto message=controlModuleSocket->readAll();
    for(auto pMessage=message.data();boxNum!=0;--boxNum,pMessage+=20)
    {
        switch(pMessage[0])
        {
        default:break;
        case 'T'://数据转发到触摸屏
            if(touchScreenSocket==nullptr)
            {
                sendMessage("1",1,controlModuleSocket);
                break;
            }
            else
            {
                sendMessage(pMessage,20,touchScreenSocket);
                sendMessage("0",1,controlModuleSocket);
                break;
            }
        case 'H'://数据转发到主机消息处理
            std::string messageSendToHost;
            char *t=pMessage;
            for(int i=0;i!=20;++i)
            {
                messageSendToHost.push_back(*t);
                t++;
            }
            emit hostMessage(messageSendToHost);
            break;
        }
    }
}

void tcpServer::incomingConnection(qintptr handle)
{
    auto socket=new clientSocket(this);
    socket->setSocketDescriptor(handle);

    /*身份校验*/
    if(socket->peerAddress().toString()==touchScreenAddr)
    {
        if(touchScreenSocket!=nullptr)
        {
            touchScreenSocket->deleteLater();
        }
        touchScreenSocket=socket;
        connect(socket,&clientSocket::disconnected,[=](){
            socket->deleteLater();
            touchScreenSocket=nullptr;
            emit deviceDisconnected(tr("触摸屏断开连接\n"));
        });

        connect(socket,&clientSocket::readyRead,[=](){
            dealTouchScreenMessage();
        });

        emit deviceConnected(tr("触摸屏已连接\n"));
    }
    else if(socket->peerAddress().toString()==controlModuleAddr)
    {
        if(controlModuleSocket!=nullptr)
        {
            controlModuleSocket->deleteLater();
        }
        controlModuleSocket=socket;
        connect(socket,&clientSocket::disconnected,[=](){
            socket->deleteLater();
            controlModuleSocket=nullptr;
            emit deviceDisconnected(tr("控制模块断开连接\n"));
        });

        connect(socket,&clientSocket::readyRead,[=](){
            dealControlModuleMessage();
        });

        emit deviceConnected(tr("控制模块已连接\n"));
    }
    else
    {
        socket->close();
    }
}
