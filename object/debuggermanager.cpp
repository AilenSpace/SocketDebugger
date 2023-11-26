#include "debuggermanager.h"

#include "tcpsocket.h"
#include "udpsocket.h"
DebuggerManager::DebuggerManager(QObject *parent)
    : QObject{parent}
{
    id=1;
    qRegisterMetaType<DebugSetting>("DebugSetting");
}

DebuggerManager::~DebuggerManager()
{
    datas.clear();
}

int DebuggerManager::createDebug(DebugSetting set)
{
    std::unique_lock lock(mu);
    std::shared_ptr<DebugObject> obj=nullptr;
    if(set.protocolType==ProtocolType::UDP_CLIENT||set.protocolType==ProtocolType::UDP_SERVRE){
        obj.reset(new UdpSocket(set));
        if(!obj->start()){
            return -1;
        }

    }else if(set.protocolType==ProtocolType::TCP_CLIENT||set.protocolType==ProtocolType::TCP_SERVRE){
        return -2;
    }else{
        return -2;
    }
    obj->setId(id);
    connect(obj.get(),&DebugObject::newData,this,&DebuggerManager::newData);
    datas.insert(id,obj);
    id++;
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
