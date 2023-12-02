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
    class DataT{
    public:
        int64_t frameId;
        QByteArray by;
    };
    explicit DebugObject(DebugSetting setting,QObject *parent = nullptr);
    virtual ~DebugObject();
    DebugSetting getSetting();
    AdvSetting getAdvSetting();
    int getId();
    QList<DataT> getRecvRecords();
    QList<DataT> getSendRecords();

    void setId(int id);
    void setAdvSetting(AdvSetting setting);
    void setReadMode(ReadMode setting);
    void setFixedReadSize(int setting);
    void setValue(ValueSetting setting);
    void setOutputFormat(IOFormat setting);
    void setAcquisitionMode(AcquisitionMode setting);
    bool getValue(ValueSetting val,QString& ret);
    bool getValue(QString& ret);

    bool isOpen();
    void clear();
    static QByteArray getFomateData(const QByteArray &data,IOFormat format);
    static QByteArray getFomateData(const QByteArray &data,IOFormat format,char separator);
    QByteArray getLastData();
    void addChildren(int id,std::shared_ptr<DebugObject> children);
    void updateChildrenId(int oldId,int newId);
    void emitNewData(QByteArray by);
    bool wirteData(const QByteArray &by,IOFormat format,QHostAddress ip,int port);
signals:
    void newData(int id,QByteArray data);
    void newChildren(int id,std::shared_ptr<DebugObject> children);
    void showError(QString error,bool pop=false);
public:
    virtual bool start()=0;
    virtual bool stop()=0;
protected:
    virtual bool write(QByteArray by,IOFormat format,QHostAddress ip,int port)=0;
protected:
    DebugSetting setting;
    bool open;
private:
    QList<DataT> recvRecords;
    QList<DataT> sendRecords;
    int64_t recvFrameId;
    int64_t sendFrameId;
    int id;
    QByteArray lastBy;
    QMap<int,std::shared_ptr<DebugObject>> childrens;
};

#endif // DEBUGOBJECT_H
