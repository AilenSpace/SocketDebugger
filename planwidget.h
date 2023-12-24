#ifndef PLANWIDGET_H
#define PLANWIDGET_H

#include <QWidget>
#include <QMap>
#include"structs.h"
namespace Ui {
class PlanWidget;
}
#include<QTableWidgetItem>
class PlanWidget : public QWidget
{
    Q_OBJECT

    explicit PlanWidget(QWidget *parent = nullptr);
public:
    static PlanWidget*instance();
    ~PlanWidget();
    void initData();
    void addPlan(Plan pl);
    void addRow(QStringList li);
    void importFunc(QString path);
signals:
    void resizeSig();
private slots:
    void on_newPlan_clicked();

    void on_deletePlan_clicked();

    void on_browseBtn_clicked();

    void on_importBtn_clicked();

    void on_exportBtn_clicked();

    void resizeEvent(QResizeEvent *event);
private:
    Ui::PlanWidget *ui;
    static PlanWidget*m_instance;
public:
    QMap<int,Plan> plans;

};

#endif // PLANWIDGET_H
