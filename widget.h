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

    void on_copyBtn_clicked();

    void setHighlight();
    void updateValue(std::shared_ptr<DebugObject> obj);
    void showError(QString error);
    void on_highlightStart_valueChanged(int arg1);

    void on_highlightEnd_valueChanged(int arg1);

    void on_stopBtn_clicked();

    void on_startBtn_clicked();

    void on_acquisitionMode_currentIndexChanged(int index);

    void on_updateHeadSet_clicked();

    void onNewChildren(int parentId,int id,std::shared_ptr<DebugObject> children);

private:
    Ui::Widget *ui;
    DebuggerManager *manager;
    int currentId;
};
#endif // WIDGET_H
