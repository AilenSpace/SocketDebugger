#include "planwidget.h"
#include "ui_planwidget.h"
#include "plandetailswidget.h"
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include<QDir>
#include<QFileInfoList>
PlanWidget*PlanWidget::m_instance=nullptr;
PlanWidget::PlanWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlanWidget)
{
    ui->setupUi(this);
    initData();
}

PlanWidget *PlanWidget::instance()
{
    if(!m_instance){
        m_instance=new  PlanWidget ;
    }
    return m_instance;
}

PlanWidget::~PlanWidget()
{
    delete ui;
}

void PlanWidget::initData()
{
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"ID"<<"Name");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setHidden(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    connect(ui->tableWidget,&QTableWidget::itemChanged,[=](QTableWidgetItem*item){
        if(item->column()==0){
            if(item->text()!=item->data(Qt::UserRole+1).toString()){
                if(plans.contains(item->text().toInt())){
                    item->setText(item->data(Qt::UserRole+1).toString());
                }else{
                    Plan pl=plans[item->data(Qt::UserRole+1).toInt()];
                    plans.remove(pl.id);
                    pl.id=item->text().toInt();
                    plans.insert(pl.id,pl);

                    item->setData(Qt::UserRole+1,QString::number(pl.id));
                }

            }

        }else if(item->column()==1){
            Plan &pl=plans[ui->tableWidget->item(item->row(),0)->text().toInt()];
            pl.name=item->text();
        }
    });

    QDir dir("./plans");
    QFileInfoList fileInfoList = dir.entryInfoList(QStringList()<<"*.plan",QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
    foreach (auto fileInfo, fileInfoList) {
        if(fileInfo.isFile())
        {
            importFunc(fileInfo.absoluteFilePath());
        }
    }
}

void PlanWidget::addPlan(Plan pl)
{
    auto checkFunc=[this](Plan &pl)->bool{
        if(plans.contains(pl.id)){
            pl.id++;
            return true;
        }

        return false;
    };
    while(checkFunc(pl));
    plans.insert(pl.id,pl);
    QStringList li;
    li<<QString("%1").arg(pl.id);
    li<<pl.name;
    addRow(li);
}

void PlanWidget::addRow(QStringList li)
{
    int row= ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    for(int i=0;i<li.size()&&ui->tableWidget->columnCount()>i;i++){
        QTableWidgetItem *item=new QTableWidgetItem;
        item->setData(Qt::UserRole+1,li[i]);
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(li[i]);
        ui->tableWidget->setItem(row,i,item);
    }
}

void PlanWidget::importFunc(QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadWrite)){
        return;
    }
    QDomDocument doc;
    doc.setContent(&file);
    file.close();
    QMap<QString,EndianType> map1;
    QMap<QString,ValueBitType> map2;
    QMap<QString,SignedType> map3;
    map1.insert("",EndianType::BIG);
    map2.insert("",ValueBitType::INT8_T);
    map3.insert("",SignedType::Unsigned);
    //value
    for(int i=(int)EndianType::MIN+1;i<(int)EndianType::MAX;i++){
        map1.insert(endianTypeToString(EndianType(i)),EndianType(i));
    }
    for(int i=(int)ValueBitType::MIN+1;i<(int)ValueBitType::MAX;i++){
        map2.insert(valueBitTypeToString(ValueBitType(i)),ValueBitType(i));
    }
    for(int i=(int)SignedType::MIN+1;i<(int)SignedType::MAX;i++){
        map3.insert(signedTypeToString(SignedType(i)),SignedType(i));
    }
    auto parseVal=[=](QDomElement &e)->ValueSetting{
        ValueSetting val;
        val.alias=e.attribute("alias","");
        val.index=e.attribute("index","0").toDouble();

        val.endianType=map1[""];
        if(map1.contains(e.attribute("endianType",""))){
            val.endianType=map1[e.attribute("endianType","")];
        }
        val.valueBitType=map2[""];
        if(map2.contains(e.attribute("valueBitType",""))){
            val.valueBitType=map2[e.attribute("valueBitType","")];
        }
        val.signedType=map3[""];
        if(map3.contains(e.attribute("signedType",""))){
            val.signedType=map3[e.attribute("signedType","")];
        }
        val.valueOffset=e.attribute("valueOffset","0").toInt();
        val.bitStart=e.attribute("bitStart","0").toInt();
        val.bitLength=e.attribute("bitLength","0").toInt();

        val.value=e.attribute("value","none");
        val.bitValue=e.attribute("bitValue","none");

        return val;
    };
    auto parsePlan=[=](QDomElement &e)->Plan{
        Plan p;
        p.name=e.attribute("name","");
        p.id=e.attribute("id","0").toDouble();
        QDomNode tmpN=e.firstChild();
        while(!tmpN.isNull()){
            QDomElement tmpE = tmpN.toElement();
            if(tmpE.nodeName()=="ValueSetting"){
                ValueSetting v=parseVal(tmpE);
                p.values.insert(v.index,v);
            }
            tmpN=tmpN.nextSibling();
        }
        return p;
    };

    QDomNode n = doc.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            qDebug() << qPrintable(e.tagName()) << Qt::endl; // the node really is an element.
        }
        if(e.nodeName()=="Plan"){
            Plan p=parsePlan(e);
            addPlan(p);
        }
        n = n.nextSibling();
    }
}

void PlanWidget::on_newPlan_clicked()
{
    Plan pl;
    pl.id=ui->tableWidget->rowCount();
    pl.name="方案";
    addPlan(pl);
}


void PlanWidget::on_deletePlan_clicked()
{
    if(ui->tableWidget->currentRow()>=0){
        plans.remove(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().toInt());
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    }
}


void PlanWidget::on_browseBtn_clicked()
{
    Plan plan;
    if(ui->tableWidget->currentRow()<0||!plans.contains(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().toInt())){
        return;
    }
    plan=plans[ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().toInt()];

    PlanDetailsWidget*wid= new PlanDetailsWidget(false,this);
    wid->setPlan(plan);
    wid->resize(this->size());
    connect(wid,&PlanDetailsWidget::backsapce,this,[=](){
        wid->hide();
        wid->deleteLater();
    });
    connect(wid,&PlanDetailsWidget::updatePlan,this,[=](Plan pl){
        if(plans.contains(pl.id)){
            plans[pl.id]=pl;
        }
    });
    connect(this,&PlanWidget::resizeSig,this,[=](){
        wid->resize(this->size());
    });
    wid->show();
}


void PlanWidget::on_importBtn_clicked()
{
    QString path=QFileDialog::getOpenFileName();
    importFunc(path);
}


void PlanWidget::on_exportBtn_clicked()
{
    Plan plan;
    if(ui->tableWidget->currentRow()<0||!plans.contains(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().toInt())){
        return;
    }
    plan=plans[ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().toInt()];
    QDomDocument doc;

    QDomElement enPlan=doc.createElement("Plan");
    enPlan.setAttribute("name",plan.name);
    enPlan.setAttribute("id",plan.id);
    for(auto iter=plan.values.begin();iter!=plan.values.end();iter++){
        ValueSetting v=iter.value();
        QDomElement val=doc.createElement("ValueSetting");
        val.setAttribute("alias",v.alias);
        val.setAttribute("index",v.index);
        val.setAttribute("endianType",endianTypeToString(v.endianType));
        val.setAttribute("valueBitType",valueBitTypeToString(v.valueBitType));
        val.setAttribute("signedType",signedTypeToString(v.signedType));
        val.setAttribute("valueOffset",v.valueOffset);
        val.setAttribute("bitStart",v.bitStart);
        val.setAttribute("bitLength",v.bitLength);
        val.setAttribute("value",v.value);
        val.setAttribute("bitValue",v.bitValue);
        enPlan.appendChild(val);
    }
    doc.appendChild(enPlan);


    QString path=QFileDialog::getSaveFileName();
    QFile file(path);
    if(!file.open(QIODevice::ReadWrite|QIODevice::Truncate)){
        return;
    }
    QTextStream stream(&file);
    doc.save(stream,4);
    file.close();
}

void PlanWidget::resizeEvent(QResizeEvent *event)
{
    emit resizeSig();
}

