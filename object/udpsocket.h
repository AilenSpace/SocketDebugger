#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QObject>
#include<QUdpSocket>
#include"debugobject.h"
class UdpSocket : public DebugObject
{
    Q_OBJECT
public:
    explicit UdpSocket(DebugSetting setting,QObject *parent = nullptr);
    virtual bool start() final;
    virtual bool stop() final;
    virtual bool write(QByteArray by,IOFormat format,QHostAddress ip,int port) final;
signals:

private:
    void onReadyRead();

private:
    QUdpSocket*socket;
    bool error;
};

#endif // UDPSOCKET_H
