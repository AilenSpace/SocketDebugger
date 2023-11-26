#include "tcpsocket.h"

TcpSocket::TcpSocket(DebugSetting setting,bool local,QObject *parent)
    : DebugObject{setting,parent}
{
    isLocal=local;
    if(isLocal){
        client=new QTcpSocket(this);
        connect(client,&QTcpSocket::readyRead,this,&TcpSocket::onReadyRead);
    }
}

bool TcpSocket::start()
{
    //不允许开始由tcpserver初始化的套接字
    if(!isLocal){
        return false;
    }
    if(ProtocolType::TCP_CLIENT==setting.protocolType){
//        if(!client->bind()){
//            qDebug()<<"绑定失败";
//            return false;
//        }
        client->connectToHost(setting.ip,setting.port);
        if(!client->waitForConnected()){
            qDebug()<<"链接失败";
            return false;
        }
    }else{
        qDebug()<<"error";
        return false;
    }
    open=true;
    return true;
}

bool TcpSocket::stop()
{
    client->disconnect();
    return true;
}

bool TcpSocket::write(QByteArray by,IOFormat format,QHostAddress ip,int port)
{
    if(open){
        client->write(getFomateData(by,format));
    }
    return true;
}

void TcpSocket::init(QTcpSocket *socket)
{
    client=socket;
    connect(client,&QTcpSocket::readyRead,this,&TcpSocket::onReadyRead);
    open=true;
}

void TcpSocket::onReadyRead()
{
    QByteArray by;
    bool first=true;
    int packageSize=0;
    while(client->bytesAvailable()>0){
        if(this->setting.readMode==ReadMode::ReadAll){
            lastBy=client->readAll();
            emit newData(id,getLastData());
        }else if(this->setting.readMode==ReadMode::Fixed){
            by+=client->read(this->setting.fixedSize-by.size());
            if(by.size()==this->setting.fixedSize){
                lastBy=by;
                by="";
                emit newData(id,getLastData());
            }
        }else if(this->setting.readMode==ReadMode::Head){
            if(first){
                by+=client->read(this->setting.head.packageSize.valueOffset-by.size());
                if(by.size()==this->setting.head.packageSize.valueOffset){
                    QString ret;
                    if(this->getValue(this->setting.head.packageSize,ret)){
                        packageSize=ret.toInt();
                        first=false;
                    }
                }
            }else{
                if(packageSize-by.size()<0){
                    first=true;
                    continue;
                }
                by+=client->read(packageSize-by.size());
                if(by.size()==packageSize){
                    lastBy=by;
                    by="";
                    emit newData(id,getLastData());
                    first=true;
                }

            }
        }

    }
}

