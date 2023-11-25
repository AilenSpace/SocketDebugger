#include "debugobject.h"
#include<QDebug>
#include"funchelper.h"
DebugObject::DebugObject(DebugSetting setting,QObject *parent)
    : QObject{parent}
{
    this->setting=setting;
    open=false;
    id=0;
    lastBy="";
}

DebugObject::~DebugObject()
{
    qDebug()<<"~DebugObject id:"<<id;
}

DebugSetting DebugObject::getDebugSetting()
{
    return setting;
}

void DebugObject::setId(int id)
{
    this->id=id;
}

int DebugObject::getId()
{
    return id;
}

void DebugObject::setHeadSetting(HeadSetting setting)
{
    this->setting.head=setting;
}

void DebugObject::setValue(ValueSetting setting)
{
    this->setting.value=setting;
}

void DebugObject::setShowFormat(ShowFormat setting)
{
    this->setting.showFormat=setting;
}

bool DebugObject::isOpen()
{
    return open;
}

bool DebugObject::getValue(ValueSetting val, QString &ret)
{
    ret="0";
    int offset=val.valueOffset-1;
    if(offset<0)return false;
    if(ValueBitType::INT8_T==val.valueBitType){
        if(offset+1>lastBy.size()){
            return false;
        }
        int8_t tmp=lastBy[offset];
        ret=QString("%1").arg(val.signedType==SignedType::Signed?tmp:(uint8_t)(tmp));
        return true;
    }else if(ValueBitType::INT16_T==val.valueBitType){
        if(offset+2>lastBy.size()){
            return false;
        }
        int16_t tmp=FuncHelper::byteToInt16(lastBy.mid(offset,2));
        ret=QString("%1").arg(val.signedType==SignedType::Signed?tmp:(uint16_t)(tmp));
        return true;
    }else if(ValueBitType::INT32_T==val.valueBitType){
        if(offset+4>lastBy.size()){
            return false;
        }
        int32_t tmp=FuncHelper::byteToInt32(lastBy.mid(offset,4));
        ret=QString("%1").arg(val.signedType==SignedType::Signed?tmp:(uint32_t)(tmp));
        return true;
    }else if(ValueBitType::FLOAT32_T==val.valueBitType){
        if(offset+4>lastBy.size()){
            return false;
        }
        float tmp=FuncHelper::byteToFloat32(lastBy.mid(offset,4));
        ret=QString("%1").arg(tmp);
        return true;
    }else if(ValueBitType::DOUBLE64_T==val.valueBitType){
        if(offset+8>lastBy.size()){
            return false;
        }
        double tmp=FuncHelper::byteToDouble64(lastBy.mid(offset,8));
        ret=QString("%1").arg(tmp);
        return true;

    }

    return false;
}

bool DebugObject::getValue(QString &ret)
{
    return getValue(this->setting.value,ret);
}

QByteArray DebugObject::getLastData()
{
    if(setting.showFormat==ShowFormat::HEX){
        return lastBy.toHex(' ');
    }else if(setting.showFormat==ShowFormat::STRING){
        return lastBy;
    }
    return lastBy;
}

bool DebugObject::start()
{
    return false;
}
