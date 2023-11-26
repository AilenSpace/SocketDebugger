#include "udpsocket.h"
#include<QByteArray>
#include<QString>
#include"funchelper.h"
UdpSocket::UdpSocket(DebugSetting setting,QObject *parent)
    : DebugObject{setting,parent}
{
    socket=new QUdpSocket(this);
    connect(socket,&QUdpSocket::readyRead,this,&UdpSocket::onReadyRead,Qt::DirectConnection);
}

bool UdpSocket::start()
{
    if(ProtocolType::UDP_SERVRE==setting.protocolType){
        if(!socket->bind(setting.ip,setting.port)){
            qDebug()<<"绑定失败";
            return false;
        }

    }else if(ProtocolType::UDP_CLIENT==setting.protocolType){

    }else{
        qDebug()<<"error";
        return false;
    }
    open=true;
    return true;
}

bool UdpSocket::stop()
{
    open=false;
    socket->abort();
    return true;
}

bool UdpSocket::write(QByteArray by,IOFormat format,QHostAddress ip,int port)
{
    if(open){
        QByteArray &&data=getFomateData(by,format);
        socket->writeDatagram(data,data.size(),ip,port);
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
        if(this->setting.acquisitionMode==AcquisitionMode::Single){
            this->stop();
            emit newData(id,getLastData());
            return;
        }
        emit newData(id,getLastData());
    }
}
