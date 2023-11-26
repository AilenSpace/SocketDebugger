#include "debuggermanager.h"
#include "tcpserver.h"
#include "tcpsocket.h"
#include "udpsocket.h"
DebuggerManager::DebuggerManager(QObject *parent)
    : QObject{parent}
{
    id=1;
    qRegisterMetaType<DebugSetting>("DebugSetting");
    qRegisterMetaType<std::shared_ptr<DebugObject>>("std::shared_ptr<DebugObject>");

}

DebuggerManager::~DebuggerManager()
{
    datas.clear();
}

int DebuggerManager::createDebug(DebugSetting set)
{
    std::unique_lock lock(mu);
    std::shared_ptr<DebugObject> obj=nullptr;
    bool autoStart=false;
    if(set.protocolType==ProtocolType::UDP_CLIENT||set.protocolType==ProtocolType::UDP_SERVRE){
        obj.reset(new UdpSocket(set));
        autoStart=true;
    }else if(set.protocolType==ProtocolType::TCP_CLIENT){
        obj.reset(new TcpSocket(set,true));
    }else if(set.protocolType==ProtocolType::TCP_SERVRE){
        obj.reset(new TcpServer(set));
        autoStart=true;
        connect(obj.get(),&DebugObject::newChildren,this,[this](int oid,std::shared_ptr<DebugObject> children){
            std::unique_lock lock(mu);
            children->setId(id);
            connect(children.get(),&DebugObject::newData,this,&DebuggerManager::newData);
            datas.insert(id,children);
            id++;
            QObject *obj=sender();
            if(obj){
                DebugObject*tmp=qobject_cast<DebugObject*>(obj);
                if(tmp){
                    tmp->updateChildrenId(oid,id-1);
                    emit this->DebuggerManager::newChildren(tmp->getId(),id-1,children);
                    return;
                }
                return;
            }
            qDebug()<<"newChildren error";
        },Qt::QueuedConnection);
    }else{
        return -2;
    }
    obj->setId(id);
    connect(obj.get(),&DebugObject::newData,this,&DebuggerManager::newData);
    datas.insert(id,obj);
    id++;
    if(autoStart)obj->start();
    return id-1;
}

std::shared_ptr<DebugObject> DebuggerManager::getDebugObject(int id)
{
    std::unique_lock lock(mu);
    if(datas.contains(id)){
        return datas[id];
    }
    return nullptr;
}

void DebuggerManager::removeDebugObject(int id)
{
    std::unique_lock lock(mu);
    if(datas.contains(id)){
        datas.remove(id);
    }
}
