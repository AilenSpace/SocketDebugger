#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include "debugobject.h"
class TcpSocket : public DebugObject
{
    Q_OBJECT
public:
    explicit TcpSocket(DebugSetting setting,QObject *parent = nullptr);
    virtual bool start() final;
    virtual bool write(QByteArray by,QHostAddress ip,int port) final;
signals:

};

#endif // TCPSOCKET_H
