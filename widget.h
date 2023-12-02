#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "object/debuggermanager.h"
#include <QTreeWidgetItem>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void initCtl();
    void initStyle();
    void initData();

    ~Widget();

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
    Ui::Widget *ui;
    DebuggerManager *manager;
};
#endif // WIDGET_H
