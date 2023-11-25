#include "udpsocket.h"
#include<QByteArray>
#include<QString>
#include"funchelper.h"
UdpSocket::UdpSocket(DebugSetting setting,QObject *parent)
    : DebugObject{setting,parent}
{

}

bool UdpSocket::start()
{
    if(ProtocolType::UDP_SERVRE==setting.protocolType){
        socket=new QUdpSocket(this);
        if(!socket->bind(setting.ip,setting.port)){
            qDebug()<<"绑定失败";
            return false;
        }
        connect(socket,&QUdpSocket::readyRead,this,&UdpSocket::onReadyRead,Qt::DirectConnection);
    }else if(ProtocolType::UDP_CLIENT==setting.protocolType){
        socket=new QUdpSocket(this);
    }else{
        qDebug()<<"error";
        return false;
    }
    open=true;
    return true;
}

bool UdpSocket::write(QByteArray by,QHostAddress ip,int port)
{
    if(open){
        socket->writeDatagram(by,by.size(),ip,port);
    }
    return true;
}
void UdpSocket::onReadyRead()
{
    if(!isOpen())return;
    while(socket->hasPendingDatagrams()){
        QByteArray by;
        by.resize(socket->pendingDatagramSize());
        socket->readDatagram(by.data(),by.size());
        qDebug()<<"onReadyRead"<<by;
        lastBy=by;
        emit newData(id,getLastData());
    }
}
