#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include "debugobject.h"
#include <QMap>
#include <QTcpServer>
#include <QTcpSocket>
class TcpSocket : public DebugObject
{
    Q_OBJECT
public:
    explicit TcpSocket(DebugSetting setting,bool local,QObject *parent = nullptr);
    virtual bool start() final;
    virtual bool stop() final;
    virtual bool write(QByteArray by,IOFormat format,QHostAddress ip,int port) final;
    void init(QTcpSocket*socket);
signals:
private:
    void onReadyRead();
private:
    QTcpSocket*client;
    bool isLocal;
};

#endif // TCPSOCKET_H
