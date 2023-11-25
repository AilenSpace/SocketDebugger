#include "tcpsocket.h"

TcpSocket::TcpSocket(DebugSetting setting,QObject *parent)
    : DebugObject{setting,parent}
{

}

bool TcpSocket::start()
{
    return true;
}

bool TcpSocket::write(QByteArray by,QHostAddress ip,int port)
{
    return true;
}
