#ifndef CREATEVALUEQUERY_H
#define CREATEVALUEQUERY_H

#include <QWidget>
#include "structs.h"
namespace Ui {
class CreateValueQuery;
}

class CreateValueQuery : public QWidget
{
    Q_OBJECT

public:
    explicit CreateValueQuery(QWidget *parent = nullptr);
    ~CreateValueQuery();
    void initData();
signals:
    void backsapce();
    void sure(ValueSetting set);
private slots:
    void on_backspaceBtn_clicked();

    void on_pushButton_clicked();

private:
    Ui::CreateValueQuery *ui;
};

#endif // CREATEVALUEQUERY_H
