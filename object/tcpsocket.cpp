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
    ProtocolType type=setting.setting->protocolType;
    if(ProtocolType::TCP_CLIENT==type){
        std::shared_ptr<TcpSetting> tcpSetting= std::dynamic_pointer_cast<TcpSetting>(setting.setting);

        if(tcpSetting->srcIp.toString()!=""&&tcpSetting->srcPort>0){
            if(!client->bind(tcpSetting->srcIp,tcpSetting->srcPort)){
                qDebug()<<"绑定失败";
                emit showError("绑定失败",true);
                return false;
            }
        }

        client->connectToHost(tcpSetting->destIp,tcpSetting->destPort);

        if(!client->waitForConnected()){
            qDebug()<<"链接失败";
            emit showError("链接失败",true);
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
    QByteArray tmpBy;
    while(client->bytesAvailable()>0){
        ReadMode mode=this->setting.advSetting.readMode;
        if(mode==ReadMode::ReadAll){
            tmpBy=client->readAll();
            emitNewData(tmpBy);
        }else if(mode==ReadMode::Fixed){
            by+=client->read(this->setting.advSetting.fixedSize-by.size());
            if(by.size()==this->setting.advSetting.fixedSize){
                tmpBy=by;
                by="";
                emitNewData(tmpBy);
            }
        }else if(mode==ReadMode::Head){
            if(first){
                by+=client->read(this->setting.advSetting.packageSize.valueOffset-by.size());
                if(by.size()==this->setting.advSetting.packageSize.valueOffset){
                    QString ret;
                    if(this->getValue(this->setting.advSetting.packageSize,ret)){
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
                    tmpBy=by;
                    by="";
                    emitNewData(tmpBy);
                    first=true;
                }

            }
        }

    }
}

