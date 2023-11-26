#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include "debugobject.h"
#include <QMap>
#include <QTcpServer>
#include <QTcpSocket>
#include <memory>
class TcpServer : public DebugObject
{
    Q_OBJECT
public:
    explicit TcpServer(DebugSetting setting,QObject *parent = nullptr);
    virtual bool start() final;
    virtual bool stop() final;
    virtual bool write(QByteArray by,IOFormat format,QHostAddress ip,int port) final;

signals:
private:
    void onNewConnection();
private:
    QTcpServer*server;
};

#endif // TCPSERVER_H
