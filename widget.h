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
    void newData(int id,QByteArray by);
    void updateUI(std::shared_ptr<DebugObject> obj);
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_sendInput_clicked();

    void on_clearInput_clicked();

    void on_clearOutput_clicked();

    void on_query_clicked();

    void on_setValSetting_clicked();

    void on_updateOutpt_clicked();

    void createDebug(DebugSetting set);

private:
    Ui::Widget *ui;
    DebuggerManager *manager;
    int currentId;
};
#endif // WIDGET_H
