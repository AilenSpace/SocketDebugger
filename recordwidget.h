#ifndef RECORDWIDGET_H
#define RECORDWIDGET_H

#include <QWidget>
#include "object/debugobject.h"
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
namespace Ui {
class RecordWidget;
}
class RecordItem: public QWidget{
    Q_OBJECT

public:
    explicit RecordItem(QWidget *parent = nullptr);
    ~RecordItem();
    void setData(DebugObject::DataT data);
    QLabel *frameId;
    QTextEdit *byEdit;
    QPushButton *copyBtn;
    QComboBox *showFormat;
    DebugObject::DataT data;
};

class RecordWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RecordWidget(QWidget *parent = nullptr);
    ~RecordWidget();
    void setData(QList<DebugObject::DataT> datas);
    void setItemHeight(int h);
private:
    Ui::RecordWidget *ui;
    int sizeH;
};

#endif // RECORDWIDGET_H
