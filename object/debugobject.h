#ifndef DEBUGOBJECT_H
#define DEBUGOBJECT_H

#include <QObject>
#include <QByteArray>
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
    void setValue(ValueSetting setting);
    void setShowFormat(ShowFormat setting);
    bool isOpen();
    bool getValue(ValueSetting val,QString& ret);
    bool getValue(QString& ret);
    virtual QByteArray getLastData();
signals:
    void newData(int id,QByteArray data);

public:
    virtual bool start()=0;
    virtual bool write(QByteArray by,QHostAddress ip,int port)=0;
protected:
    DebugSetting setting;
    bool open;
    int id;
    QByteArray lastBy;
};

#endif // DEBUGOBJECT_H