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
    ProtocolType type=setting.setting->protocolType;
    if(ProtocolType::UDP_SERVRE==type||ProtocolType::UDP_CLIENT==type)
    {
        std::shared_ptr<UdpSetting> udpSetting= std::dynamic_pointer_cast<UdpSetting>(setting.setting);
        if(udpSetting->srcIp.toString()!=""&&udpSetting->srcPort>0){
            if(!socket->bind(udpSetting->srcIp,udpSetting->srcPort)){
                qDebug()<<"绑定失败";
                 emit showError("绑定失败",true);
                return false;
            }
        }
    }


    else{
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
    QByteArray tmpBy;
    while(socket->hasPendingDatagrams()){
        QByteArray by;
        by.resize(socket->pendingDatagramSize());
        socket->readDatagram(by.data(),by.size());
        qDebug()<<"onReadyRead"<<by;
        tmpBy=by;
        if(this->setting.acquisitionMode==AcquisitionMode::Single){
            this->stop();
            emitNewData(tmpBy);
            return;
        }
        emitNewData(tmpBy);
    }
}
