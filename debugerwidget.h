#ifndef DEBUGERWIDGET_H
#define DEBUGERWIDGET_H

#include <QWidget>
#include "structs.h"
#include "object/debugobject.h"
#include <memory>
namespace Ui {
class DebugerWidget;
}

class DebugerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DebugerWidget(QWidget *parent = nullptr);
    int getId();
    ~DebugerWidget();
    void setDebugObject(std::shared_ptr<DebugObject> obj);
signals:
    void showError(QString error,bool pop=false);
private:
    void initData();
    void updateUI(std::shared_ptr<DebugObject> obj);
    void updateInfo(const DebugSetting &set);
    void updateValueQuery(const DebugSetting &set);
    void updateSetting(const DebugSetting &set);
    void updateAdvSetting(const DebugSetting &set);
    void refreshHighlight();
    void updateValue(std::shared_ptr<DebugObject> obj);

private slots:
    void on_setValSetting_clicked();

    void on_query_clicked();

    void on_copyBtn_clicked();

    void on_clearOutputBtn_clicked();

    void on_highlightStart_textChanged(const QString &arg1);

    void on_highlightLength_textChanged(const QString &arg1);

    void on_sendBtn_clicked();

    void on_clearInputBtn_clicked();

    void on_updateAdvSet_clicked();

    void on_startBtn_clicked();

    void on_stopBtn_clicked();

    void on_acquisitionMode_activated(int index);

    void on_advBitType_activated(int index);

    void on_bitType_activated(int index);

    void onNewData(int id,QByteArray by);

    void on_outputFormat_activated(int index);

    void on_historyRecv_clicked();

    void on_historySend_clicked();

private:
    Ui::DebugerWidget *ui;
    std::shared_ptr<DebugObject> object;

};

#endif // DEBUGERWIDGET_H
