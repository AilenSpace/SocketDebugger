#include "plandetailswidget.h"
#include "ui_plandetailswidget.h"
#include "createvaluequery.h"
PlanDetailsWidget::PlanDetailsWidget(bool showQuery,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlanDetailsWidget)
{
    ui->setupUi(this);
    this->showQuery= showQuery;
    initData();
}

PlanDetailsWidget::~PlanDetailsWidget()
{
    delete ui;
}

void PlanDetailsWidget::initData()
{
    QStringList li;
    li<<"Index"<<"Alias"<<"Offset"<<"Signed"<<"Type"<<"Endian"<<"BitLength"<<"BitEnd";
    if(showQuery){
        li <<"value"<<"BitValue";
        ui->addQuery->setVisible(false);
        ui->deleteQuery->setVisible(false);
    }
    ui->tableWidget->setColumnCount(li.size());
    ui->tableWidget->setHorizontalHeaderLabels(li);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setHidden(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui->tableWidget->setEditTriggers(QTableWidget::EditTrigger::NoEditTriggers);
}

void PlanDetailsWidget::setPlan(Plan pl)
{
    for(auto iter=pl.values.begin();iter!=pl.values.end();iter++){
        addRow(iter.value());
    }
    plan=pl;
}

void PlanDetailsWidget::addRow(QStringList li)
{
    int row= ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    for(int i=0;i<li.size()&&ui->tableWidget->columnCount()>i;i++){
        QTableWidgetItem *item=new QTableWidgetItem;
        item->setText(li[i]);
        ui->tableWidget->setItem(row,i,item);
    }
}

void PlanDetailsWidget::addRow(const ValueSetting &set)
{
    ValueSetting v=set;
    QStringList li;
    li<<QString("%1").arg(v.index);
    li<<QString("%1").arg(v.alias);
    li<<QString("%1").arg(v.valueOffset);
    li<<QString("%1").arg(signedTypeToString(v.signedType));
    li<<QString("%1").arg(valueBitTypeToString(v.valueBitType));
    li<<QString("%1").arg(endianTypeToString(v.endianType));
    li<<QString("%1").arg(v.bitStart);
    li<<QString("%1").arg(v.bitLength);
    if(showQuery){
        li<<QString("%1").arg(v.value);
        li<<QString("%1").arg(v.bitValue);
    }
    if( plan.values.contains(set.index)){
        plan.values[set.index]=set;
        updateRow(li);
    }else{
        plan.values.insert(set.index,set);
        addRow(li);
    }
    emit updatePlan(plan);
}

void PlanDetailsWidget::updateRow(QStringList li)
{
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        if(ui->tableWidget->item(i,0)->text()==li[0]){

            for(int j=1;j<ui->tableWidget->columnCount()&&li.size()>j;j++){
                QTableWidgetItem *item=ui->tableWidget->item(i,j);
                item->setTextAlignment(Qt::AlignCenter);
                item->setText(li[j]);
            }
        }
    }
}

void PlanDetailsWidget::on_backspace_clicked()
{
    emit backsapce();
}


void PlanDetailsWidget::on_addQuery_clicked()
{
    CreateValueQuery *wid=new CreateValueQuery(this);
    wid->resize(this->size());
    connect(wid,&CreateValueQuery::backsapce,this,[=](){
        wid->hide();
        wid->deleteLater();
    });
    connect(wid,&CreateValueQuery::sure,this,[=](ValueSetting set){
        wid->hide();
        wid->deleteLater();
        addRow(set);
        ui->tableWidget->sortItems(0);
        emit updatePlan(plan);
    });
    connect(this,&PlanDetailsWidget::resizeSig,this,[=](){
        wid->resize(this->size());
    });
    wid->show();
}


void PlanDetailsWidget::on_deleteQuery_clicked()
{
    if(ui->tableWidget->currentRow()>=0){
        plan.values.remove(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().toDouble());
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
        emit updatePlan(plan);
    }
}

void PlanDetailsWidget::resizeEvent(QResizeEvent *event)
{
    emit resizeSig();
}

