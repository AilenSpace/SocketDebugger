#include "tcpserver.h"

TcpServer::TcpServer(DebugSetting setting,QObject *parent)
    : DebugObject{setting,parent}
{
    server=new QTcpServer(this);

    connect(server,&QTcpServer::newConnection,this,&TcpServer::onNewConnection,Qt::DirectConnection);

}

bool TcpServer::start()
{
    ProtocolType type=setting.setting->protocolType;
    if(ProtocolType::TCP_SERVRE==type){
        std::shared_ptr<TcpSetting> tcpSetting= std::dynamic_pointer_cast<TcpSetting>(setting.setting);
        if(!server->listen(tcpSetting->srcIp,tcpSetting->srcPort)){
            qDebug()<<"监听失败";
            emit showError("监听失败",true);
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
        addChildren(getId()+i,tmp);
        emit newChildren(getId()+i,tmp);
    }
}
