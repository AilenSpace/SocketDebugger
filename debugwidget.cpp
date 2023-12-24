#include "debugwidget.h"
#include "./ui_debugwidget.h"
#include <QPushButton>
#include<QLabel>
#include<QClipboard>
#include<QMessageBox>
#include<QMenuBar>
#include "createobject.h"
#include "debugerwidget.h"
DebugWidget::DebugWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DebugWidget)
{
    ui->setupUi(this);
    initCtl();
    initData();
}

void DebugWidget::initCtl()
{
    resize(1400,1000);
    QList<int> sizes;
    sizes<<2500<<7500;
    ui->splitter->setSizes(sizes);
    sizes.clear();
    sizes<<900<<100;
    ui->splitter_2->setSizes(sizes);
    ui->treeWidget->setColumnCount(1);
    connect(ui->treeWidget,&BasicTreeWidget::deleteItemed,this,[this](QVariant var){
        if(manager)
            manager->removeDebugObject(var.toInt());
    });

}


void DebugWidget::initData()
{
    manager=new DebuggerManager;
    connect(manager,&DebuggerManager::newChildren,this,&DebugWidget::onNewChildren);
    connect(manager,&DebuggerManager::newData,this,&DebugWidget::newData);
    connect(manager,&DebuggerManager::showError,this,&DebugWidget::showError);
//    ValueSetting val;
//    val.valueBitType=ValueBitType::INT8_T;
//    val.endianType=EndianType::SMALL;
//    val.signedType=SignedType::Unsigned;
//    val.valueOffset=0;


//    AdvSetting adv;
//    adv.readMode=ReadMode::ReadAll;
//    adv.packageSize=val;
//    adv.fixedSize=0;
//    adv.packageSize=val;

//    std::shared_ptr<BasicSetting>  set;
//    std::shared_ptr<UdpSetting> udp=std::make_shared<UdpSetting>();
//    udp->srcIp=QHostAddress("127.0.0.1");
//    udp->srcPort=9060;
//    set=udp;
//    set->protocolType=ProtocolType::UDP_SERVRE;;
//    DebugSetting setting;
//    setting.oFormat=IOFormat::TO_HEX;
//    setting.advSetting=adv;
//    setting.value=val;
//    setting.setting=set;

//    this->createDebug(setting);

//    udp=std::make_shared<UdpSetting>();
//    set=udp;
//    set->protocolType=ProtocolType::UDP_CLIENT;;
//    udp->srcIp=QHostAddress("127.0.0.1");
//    udp->srcPort=9061;
//    setting.setting=set;
//    this->createDebug(setting);

}

DebugWidget::~DebugWidget()
{
    delete ui;
    delete manager;
}

void DebugWidget::on_createObject_clicked()
{
    CreateObject *widget=new CreateObject();
    widget->setWindowTitle("创建调试对象");
    QString style=QString("QWidget{background-color :rgb(224,251,255);font-size:12pt}"
                          "QWidget#boardFarme,QLineEdit,QTextEdit#inputEdit,QTextEdit#outputLab,QTabWidget::pane,QComboBox"
                          "{border:1px solid rgb(192,192,192); border-radius:2px}"
                          "QPushButton{background-color :rgb(230,240,234);border-radius:6px}"
                          "QPushButton::pressed{background-color :rgb(200,233,255);}"
                          "QPushButton::hover{background-color :rgb(146,240,234);}"
                          "QTreeWidget{font-size:6pt;}"
                          "QTabWidget QTabBar::tab{background-color :rgb(206,230,234);}"
                          "QTabWidget QTabBar::tab:selected{background-color :rgb(230,240,234);}"
                          );
    widget->setStyleSheet(style);
    connect(widget,&CreateObject::createDebug,this,&DebugWidget::createDebug);
    widget->setAttribute(Qt::WA_DeleteOnClose,true);
    widget->show();
}


void DebugWidget::on_deleteObject_clicked()
{
    if(ui->treeWidget->currentIndex().row()>=0){

        QVariant var=ui->treeWidget->currentItem()->data(0,Qt::DisplayRole+1);
        int id=var.toInt();
        int index=-1;
        DebugerWidget *debugerWidget=nullptr;
        for(int i=0;i<ui->tabWidget->count();i++){
            QWidget *wid= ui->tabWidget->widget(i);

            DebugerWidget *tmp=qobject_cast<DebugerWidget*>(wid);
            if(tmp){
                if(tmp->getId()==id&&id>0){
                    debugerWidget=tmp;
                    index=i;
                    break;
                }
            }
        }
        if(index>=0){
            ui->tabWidget->removeTab(index);
            if(debugerWidget)
                delete debugerWidget;
        }
        ui->treeWidget->clear(ui->treeWidget->currentItem());
    }else{
        showError("未选择删除节点");
    }
}

void DebugWidget::newData(int id, QByteArray by)
{
    QVariant var=ui->treeWidget->currentItem()->data(0,Qt::DisplayRole+1);
}

void DebugWidget::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QVariant var=item->data(0,Qt::DisplayRole+1);
    int id=var.toInt();
    int index=-1;
    for(int i=0;i<ui->tabWidget->count();i++){
        QWidget *wid= ui->tabWidget->widget(i);

        DebugerWidget *debugerWidget=qobject_cast<DebugerWidget*>(wid);
        if(debugerWidget){
            if(debugerWidget->getId()==id&&id>0){
                index=i;
                break;
            }
        }
    }
    if(index>=0){
        ui->tabWidget->setCurrentIndex(index);
    }else{
        std::shared_ptr<DebugObject> obj=manager->getDebugObject(id);
        if(obj){
            DebugerWidget *debugerWidget=new DebugerWidget(ui->tabWidget);
            connect(debugerWidget,&DebugerWidget::showError,this,&DebugWidget::showError);
            debugerWidget->setDebugObject(obj);
            ui->tabWidget->addTab(debugerWidget,protocolTypeToString(obj->getSetting().setting->protocolType)+QString::number(id));
            ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

        }

    }

}

void DebugWidget::createDebug(DebugSetting set)
{
    int ret=-1;
    if((ret=manager->createDebug(set))>0){
        QList<QTreeWidgetItem*> items;
        QTreeWidgetItem*item=new QTreeWidgetItem;
        item->setText(0,protocolTypeToString(set.setting->protocolType)+QString::number(ret));
        item->setData(0,Qt::DisplayRole+1,ret);
        qDebug()<<"createDebug:"<<ret;
        items<<item;
        ui->treeWidget->appendItem(nullptr,items);
    }else if(ret==-2){
        showError("暂不支持该类型\n",true);
    }else{
        showError("创建失败",true);
    }
}


void DebugWidget::showError(QString error,bool pop)
{
    ui->logInfo->append(error+"\n");
    if(pop)
        QMessageBox::warning(nullptr,"错误",error);
}


void DebugWidget::onNewChildren(int parentId, int id, std::shared_ptr<DebugObject> children)
{
    QTreeWidgetItem *parentItem=ui->treeWidget->findId(parentId);

    QList<QTreeWidgetItem*> items;
    QTreeWidgetItem*item=new QTreeWidgetItem(parentItem);
    item->setText(0,protocolTypeToString(children->getSetting().setting->protocolType)+QString::number(children->getId()));
    item->setData(0,Qt::DisplayRole+1,children->getId());
    items<<item;
    qDebug()<<"onNewChildren"<<parentId<<id;
    ui->treeWidget->appendItem(parentItem,items);
}


void DebugWidget::on_tabWidget_tabCloseRequested(int index)
{
    QWidget *wid= ui->tabWidget->widget(index);

    DebugerWidget *tmp=qobject_cast<DebugerWidget*>(wid);

    ui->tabWidget->removeTab(index);
    if(tmp){
        delete tmp;
    }
}

void DebugWidget::actionAddValueQuery()
{

}

