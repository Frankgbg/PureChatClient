#include "socketthread.h"

SocketThread::SocketThread(Socket *socket, QObject *parent) :
    QThread(parent)
{
    connectStatus = true;

    _socket = socket;
    connect(_socket,SIGNAL(connected()),this,SLOT(connection()));
    connect(_socket,SIGNAL(disconnected()),this,SLOT(disconnection()));

    timer.setInterval(60000);
    connect(&timer,SIGNAL(timeout()),this,SLOT(connectServer()));
    timer.start();
    moveToThread(this);
}

void SocketThread::run()
{
    this->exec();
}

void SocketThread::login()
{
    connectServer();
    timer.start();
}

void SocketThread::setStop()
{
    timer.stop();
    connectStatus = false;
    exit();
}

void SocketThread::connection()
{
    connectStatus = true;
}

void SocketThread::disconnection()
{
    connectStatus = false;
}

void SocketThread::connectServer()
{
    if(!connectStatus){
        qDebug()<<"TCP重连";
        _socket->abort();
        _socket->connectToHost("47.96.226.11",61314);
        if(_socket->waitForConnected(500)){
            qDebug()<<"重连成功";
        }
    }
    else{
        qDebug()<<"TCP在线";
        _socket->sendData("");
    }
}
