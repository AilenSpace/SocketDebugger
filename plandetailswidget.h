#ifndef PLANDETAILSWIDGET_H
#define PLANDETAILSWIDGET_H

#include <QWidget>
#include "structs.h"
namespace Ui {
class PlanDetailsWidget;
}

class PlanDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlanDetailsWidget(bool showQuery,QWidget *parent = nullptr);
    ~PlanDetailsWidget();
    void initData();
    void setPlan(Plan pl);
    void addRow(QStringList li);
    void addRow(const ValueSetting &set);
    void updateRow(QStringList li);
signals:
    void backsapce();
    void updatePlan(Plan pl);
    void resizeSig();
private slots:
    void on_backspace_clicked();

    void on_addQuery_clicked();

    void on_deleteQuery_clicked();

    void resizeEvent(QResizeEvent *event);
private:
    Ui::PlanDetailsWidget *ui;
    Plan plan;
    bool showQuery;
};

#endif // PLANDETAILSWIDGET_H
