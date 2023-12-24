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
    sendFrameId=0;
    recvFrameId=0;
}

DebugObject::~DebugObject()
{
    qDebug()<<"~DebugObject id:"<<id;
    childrens.clear();
}

DebugSetting DebugObject::getSetting()
{
    return setting;
}

AdvSetting DebugObject::getAdvSetting()
{
    return setting.advSetting;
}

void DebugObject::setId(int id)
{
    this->id=id;
}

int DebugObject::getId()
{
    return id;
}

QList<DebugObject::DataT> DebugObject::getRecvRecords()
{
    return recvRecords;
}

QList<DebugObject::DataT> DebugObject::getSendRecords()
{
    return sendRecords;
}

void DebugObject::setAdvSetting(AdvSetting setting)
{
    this->setting.advSetting=setting;
}

void DebugObject::setReadMode(ReadMode setting)
{
    this->setting.advSetting.readMode=setting;
}

void DebugObject::setFixedReadSize(int setting)
{
    this->setting.advSetting.fixedSize=setting;
}

void DebugObject::setValue(ValueSetting setting)
{
    this->setting.value=setting;
}

void DebugObject::setOutputFormat(IOFormat setting)
{
    this->setting.oFormat=setting;
}

void DebugObject::setAcquisitionMode(AcquisitionMode setting)
{
    this->setting.acquisitionMode=setting;
}

bool DebugObject::isOpen()
{
    return open;
}

bool DebugObject::getValue(ValueSetting val, QString &ret)
{
    ret="查询失败";
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

template <class  T>
T calcBitValue(T i,int index,int n)
{
    int leftShift=sizeof(T)*8-(index+n);
    i<<=leftShift;
    i>>=index+leftShift;
    return i;
};

bool DebugObject::getValue(const ValueSetting &val, ValueSetting &ret)
{
    ret=val;
    if(getValue(val, ret.value)){
        QString res=ret.value;
        int start=val.bitStart;
        int count=val.bitLength;
        ValueBitType type=val.valueBitType;

        if(type==ValueBitType::INT8_T){
            uint8_t v= res.toInt();
            ret.bitValue=QString("%1").arg(calcBitValue(v,start,count));
        } else   if(type==ValueBitType::INT16_T){
            uint16_t v= res.toInt();
            ret.bitValue=QString("%1").arg(calcBitValue(v,start,count));
        }else  if(type==ValueBitType::INT32_T){
            uint32_t v= res.toInt();
            ret.bitValue=QString("%1").arg(calcBitValue(v,start,count));
        }else{
            ret.bitValue="不支持的类型查询";
        }
        return true;
    }

    return false;
}

void DebugObject::getValue(const Plan &val, Plan &ret)
{
    ret=val;
    for(auto iter=val.values.begin();iter!=val.values.end();iter++){
        getValue(iter.value(), ret.values[iter.key()]);
    }
}

bool DebugObject::getValue(QString &ret)
{
    return getValue(this->setting.value,ret);
}

void DebugObject::clear()
{
    lastBy="";
    recvFrameId=0;
    recvRecords.clear();
    sendFrameId=0;
    sendRecords.clear();
}

QByteArray DebugObject::getFomateData(const QByteArray &data, IOFormat format)
{
    if(format==IOFormat::TO_HEX){
        return data.toHex();
    }else if(format==IOFormat::FROM_HEX){
        return QByteArray::fromHex(data);
    }else if(format==IOFormat::BYTE_ARRAY){
        return data;
    }
    return data;
}

QByteArray DebugObject::getFomateData(const QByteArray &data, IOFormat format, char separator)
{
    if(format==IOFormat::TO_HEX){
        return data.toHex(separator);
    }else if(format==IOFormat::FROM_HEX){
        return QByteArray::fromHex(data);
    }else if(format==IOFormat::BYTE_ARRAY){
        return data;
    }
    return data;
}

QByteArray DebugObject::getLastData()
{
    return getFomateData(lastBy,setting.oFormat,' ');
}

void DebugObject::addChildren(int id, std::shared_ptr<DebugObject> children)
{
    childrens.insert(id,children);
}

void DebugObject::updateChildrenId(int oldId, int newId)
{
    if(childrens.contains(oldId)){
        std::shared_ptr<DebugObject> tmp=childrens[oldId];
        childrens.remove(oldId);
        childrens.insert(newId,tmp);
    }
}

void DebugObject::emitNewData(QByteArray by)
{
    lastBy=by;
    DataT data;
    data.by=by;
    data.frameId=recvFrameId++;
    recvRecords.append(data);
    emit newData(id,getFomateData(lastBy,setting.oFormat,' '));
}

bool DebugObject::wirteData(const QByteArray &by, IOFormat format, QHostAddress ip, int port)
{
    DataT data;
    data.by=by;
    data.frameId=sendFrameId++;
    sendRecords.append(data);
    return this->write(by,format,ip,port);
}

bool DebugObject::start()
{

    return false;
}
