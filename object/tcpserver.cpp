#include "tcpserver.h"

TcpServer::TcpServer(DebugSetting setting,QObject *parent)
    : DebugObject{setting,parent}
{
    server=new QTcpServer(this);

    connect(server,&QTcpServer::newConnection,this,&TcpServer::onNewConnection,Qt::DirectConnection);

}

bool TcpServer::start()
{
    if(ProtocolType::TCP_SERVRE==setting.protocolType){
        if(!server->listen(setting.ip,setting.port)){
            qDebug()<<"监听失败";
            return false;
        }
    }else{
        qDebug()<<"error";
        return false;
    }
    open=true;
    return true;
}

bool TcpServer::stop()
{
    server->close();
    return true;
}

bool TcpServer::write(QByteArray by, IOFormat format, QHostAddress ip, int port)
{
    return false;
}
#include "tcpsocket.h"
void TcpServer::onNewConnection()
{
    static int i=1;
    i++;
    while(server->hasPendingConnections())
    {
        qDebug()<<"hasPendingConnections";
        QTcpSocket *socket=server->nextPendingConnection();
        std::shared_ptr<TcpSocket> tmp;
        tmp.reset(new TcpSocket(this->setting,false));
        tmp->init(socket);
        addChildren(id+i,tmp);
        emit newChildren(id+i,tmp);
    }
}
