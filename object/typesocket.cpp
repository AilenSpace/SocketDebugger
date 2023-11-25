#include "debugobject.h"

DebugObject::DebugObject(SocketSetting setting,QObject *parent)
    : QObject{parent}
{
    this->setting=setting;
    open=false;
}

void DebugObject::setHeadSetting(HeadSetting setting)
{
    this->setting.head=setting;
}

bool DebugObject::isOpen()
{
    return open;
}

bool DebugObject::start()
{
    return false;
}
