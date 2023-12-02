#ifndef DEBUGGERMANAGER_H
#define DEBUGGERMANAGER_H

#include <QObject>
#include <QMap>
#include <memory>
#include <mutex>
#include "debugobject.h"
class DebuggerManager : public QObject
{
    Q_OBJECT
public:
    explicit DebuggerManager(QObject *parent = nullptr);
    ~DebuggerManager();
    int createDebug(DebugSetting set);
    std::shared_ptr<DebugObject> getDebugObject(int id);
    void removeDebugObject(int id);
signals:
    int newData(int id,QByteArray by);
    void newChildren(int parentId,int id,std::shared_ptr<DebugObject> children);
    void showError(QString error,bool pop);
private:
    int id;
    QMap<int,std::shared_ptr<DebugObject>> datas;
    std::mutex mu;

};

#endif // DEBUGGERMANAGER_H
