#ifndef DEBUGOBJECT_H
#define DEBUGOBJECT_H

#include <QObject>
#include <QByteArray>
#include <QMap>
#include"../structs.h"
class DebugObject : public QObject
{
    Q_OBJECT
public:
    explicit DebugObject(DebugSetting setting,QObject *parent = nullptr);
    virtual ~DebugObject();
    DebugSetting getDebugSetting();
    void setId(int id);
    int getId();
    void setHeadSetting(HeadSetting setting);
    void setReadMode(ReadMode setting);
    void setFixedReadSize(int setting);
    void setValue(ValueSetting setting);
    void setOutputFormat(IOFormat setting);
    void setAcquisitionMode(AcquisitionMode setting);
    bool isOpen();
    bool getValue(ValueSetting val,QString& ret);
    bool getValue(QString& ret);
    QByteArray getFomateData(const QByteArray &data,IOFormat format);
    QByteArray getFomateData(const QByteArray &data,IOFormat format,char separator);
    QByteArray getLastData();
    void addChildren(int id,std::shared_ptr<DebugObject> children);
    void updateChildrenId(int oldId,int newId);
signals:
    void newData(int id,QByteArray data);
    void newChildren(int id,std::shared_ptr<DebugObject> children);
public:
    virtual bool start()=0;
    virtual bool stop()=0;
    virtual bool write(QByteArray by,IOFormat format,QHostAddress ip,int port)=0;
protected:
    DebugSetting setting;
    bool open;
    int id;
    QByteArray lastBy;
    QMap<int,std::shared_ptr<DebugObject>> childrens;
};

#endif // DEBUGOBJECT_H
