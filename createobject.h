#ifndef CREATEOBJECT_H
#define CREATEOBJECT_H

#include <QWidget>
#include"structs.h"
namespace Ui {
class CreateObject;
}

class CreateObject : public QWidget
{
    Q_OBJECT

public:
    explicit CreateObject(QWidget *parent = nullptr);
    ~CreateObject();
    void initData();
signals:
    void createDebug(DebugSetting set);
private slots:
    void on_create_clicked();

    void on_cancel_clicked();

    void on_comboBoxProto_currentIndexChanged(int index);

    void on_comboBoxProto_currentTextChanged(const QString &arg1);

private:
    Ui::CreateObject *ui;
};

#endif // CREATEOBJECT_H
