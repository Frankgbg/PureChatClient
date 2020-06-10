#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include "socket.h"

#include <QThread>
#include <QTimer>

class SocketThread : public QThread
{
    Q_OBJECT
public:
    SocketThread(Socket *socket, QObject *parent = nullptr);

    void run() override;

public:
    void login();
    void setStop();

private slots:
    void connection();
    void disconnection();
    void connectServer();

private:
    Socket *_socket;
    QTimer timer;
    bool connectStatus;
};

#endif // SOCKETTHREAD_H
