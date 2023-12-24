#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "object/debuggermanager.h"
#include <QTreeWidgetItem>
#include <functional>
QT_BEGIN_NAMESPACE
namespace Ui { class DebugWidget; }
QT_END_NAMESPACE

class DebugWidget : public QWidget
{
    Q_OBJECT

public:
    DebugWidget(QWidget *parent = nullptr);
    void initCtl();
    void initData();

    ~DebugWidget();
private slots:
    void on_createObject_clicked();

    void on_deleteObject_clicked();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void newData(int id,QByteArray by);

    void onNewChildren(int parentId,int id,std::shared_ptr<DebugObject> children);

    void createDebug(DebugSetting set);

    void showError(QString error,bool pop=false);

    void on_tabWidget_tabCloseRequested(int index);
private:
    void actionAddValueQuery();
private:
    Ui::DebugWidget *ui;
    DebuggerManager *manager;
};
#endif // WIDGET_H
