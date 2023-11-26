#include "widget.h"
#include "./ui_widget.h"
#include <QPushButton>
#include<QLabel>
#include<QClipboard>
#include<QMessageBox>
#include "createobject.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    initCtl();
    initStyle();
    initData();
}

void Widget::initCtl()
{
    resize(1280,800);
    QList<int> sizes;
    sizes<<2000<<8000;
    ui->splitterH->setSizes(sizes);
    sizes.clear();
    sizes<<4000<<4000<<2000;
    ui->splitterV->setSizes(sizes);

    ui->treeWidget->setColumnCount(1);
    connect(ui->treeWidget,&BasicTreeWidget::deleteItemed,this,[this](QVariant var){
        if(manager)
            manager->removeDebugObject(var.toInt());
    });
}

void Widget::initStyle()
{

    this->setObjectName("boardFarme");
    QString style=QString("QWidget{background-color :rgb(224,251,255);font-size:12pt}"
                          "QWidget#boardFarme,QLineEdit,QTextEdit#inputEdit,QTextEdit#outputLab,QTabWidget::pane,QComboBox"
                          "{border:1px solid rgb(192,192,192); border-radius:2px}"
                          "QSplitter::handle{border: 1px solid rgb(192,192,192);width:1px}"
                          "QPushButton{background-color :rgb(230,240,234);border-radius:6px}"
                          "QPushButton::pressed{background-color :rgb(200,233,255);}"
                          "QPushButton::hover{background-color :rgb(146,240,234);}"
                          "QTreeWidget{font-size:10pt;}"
                          "QTabWidget QTabBar::tab{background-color :rgb(206,230,234);}"
                          "QTabWidget QTabBar::tab:selected{background-color :rgb(230,240,234);}"
                          );
    this->setStyleSheet(style);

}

void Widget::initData()
{
    manager=new DebuggerManager;
    ui->inputEdit->setText("");
    ui->outputLab->setText("");
    for(int i=(int)ProtocolType::MIN;i<(int)ProtocolType::MAX;i++){
        ui->comboBoxProto->addItem(protocolTypeToString(ProtocolType(i)),i);
    }
    for(int i=(int)IOFormat::MIN;i<(int)IOFormat::MAX;i++){
        ui->comboBoxOutFormat->addItem(IOFormatToString(IOFormat(i)),i);
    }
    for(int i=(int)IOFormat::MIN+1;i<(int)IOFormat::MAX;i++){
        ui->inputFormat->addItem(IOFormatToString(IOFormat(i)),i);
    }
    ui->inputFormat->setCurrentText(IOFormatToString(IOFormat::BYTE_ARRAY));


    for(int i=(int)ValueBitType::MIN;i<(int)ValueBitType::MAX;i++){
        ui->bitType->addItem(valueBitTypeToString(ValueBitType(i)),i);
    }
    for(int i=(int)EndianType::MIN;i<(int)EndianType::MAX;i++){
        ui->comboBoxEndian->addItem(endianTypeToString(EndianType(i)),i);
    }
    for(int i=(int)SignedType::MIN;i<(int)SignedType::MAX;i++){
        ui->signedType->addItem(signedTypeToString(SignedType(i)),i);
    }


    ui->sendIP->setText("127.0.0.1");
    ui->sendPort->setText("9060");
    ui->currentLable->setText("当前:-1");
    currentId=0;





    ValueSetting val;
    val.valueBitType=ValueBitType::INT8_T;
    val.endianType=EndianType::SMALL;
    val.signedType=SignedType::Signed;
    val.valueOffset=1;
    DebugSetting setting;
    setting.ip=QHostAddress("127.0.0.1");
    setting.port=9060;
    setting.oFormat=IOFormat::BYTE_ARRAY;

    HeadSetting head;
    head.hasPacketHead=false;
    head.fullPacketSize=true;//是否是包含 包头的大小
    head.packageSize=val;//形容包大小

    setting.head=head;
    setting.value=val;

    setting.protocolType=ProtocolType::UDP_SERVRE;
    this->createDebug(setting);
    setting.protocolType=ProtocolType::UDP_CLIENT;
    this->createDebug(setting);
}

Widget::~Widget()
{
    delete ui;
    delete manager;
}


void Widget::on_createObject_clicked()
{
    CreateObject *widget=new CreateObject();
    widget->setWindowTitle("创建调试对象");
    QString style=QString("QWidget{background-color :rgb(224,251,255);font-size:12pt}"
                          "QWidget#boardFarme,QLineEdit,QTextEdit#inputEdit,QTextEdit#outputLab,QTabWidget::pane,QComboBox"
                          "{border:1px solid rgb(192,192,192); border-radius:2px}"
                          "QPushButton{background-color :rgb(230,240,234);border-radius:6px}"
                          "QPushButton::pressed{background-color :rgb(200,233,255);}"
                          "QPushButton::hover{background-color :rgb(146,240,234);}"
                          "QTreeWidget{font-size:10pt;}"
                          "QTabWidget QTabBar::tab{background-color :rgb(206,230,234);}"
                          "QTabWidget QTabBar::tab:selected{background-color :rgb(230,240,234);}"
                          );
    widget->setStyleSheet(style);
    connect(widget,&CreateObject::createDebug,this,&Widget::createDebug);
    widget->setAttribute(Qt::WA_DeleteOnClose,true);
    widget->show();
}


void Widget::on_deleteObject_clicked()
{
    if(ui->treeWidget->currentIndex().row()>=0){
        ui->treeWidget->clear(ui->treeWidget->currentItem());
    }else{
        showError("未选择删除节点");
    }
}

void Widget::newData(int id, QByteArray by)
{
    QVariant var=ui->treeWidget->currentItem()->data(0,Qt::DisplayRole+1);
    if(id!=var.toInt())return;
    ui->outputLab->setText(by);

    std::shared_ptr<DebugObject> obj=manager->getDebugObject(currentId);
    if(obj){
        updateValue(obj);
    }else{

    }
}

void Widget::updateUI(std::shared_ptr<DebugObject> obj)
{
    DebugSetting set=  obj->getDebugSetting();
    ui->currentLable->setText("当前:"+protocolTypeToString(set.protocolType)+QString::number(obj->getId()));
    ui->lineEditProtoIP->setText(set.ip.toString());
    ui->lineEditProtoPort->setText(QString::number(set.port));
    ui->comboBoxProto->setCurrentText(protocolTypeToString(set.protocolType));
    ui->comboBoxOutFormat->setCurrentText(IOFormatToString(set.oFormat));

    ui->bitType->setCurrentText(valueBitTypeToString(set.value.valueBitType));
    ui->comboBoxEndian->setCurrentText(endianTypeToString(set.value.endianType));
    ui->signedType->setCurrentText(signedTypeToString(set.value.signedType));
    ui->lineEditOffset->setText(QString::number(set.value.valueOffset));
    updateValue(obj);
    if(set.protocolType==ProtocolType::UDP_CLIENT){
        ui->sendIP->setVisible(true);
        ui->sendPort->setVisible(true);
        ui->labelIP->setVisible(true);
        ui->labelPort->setVisible(true);
    }else{
        ui->sendIP->setVisible(false);
        ui->sendPort->setVisible(false);
        ui->labelIP->setVisible(false);
        ui->labelPort->setVisible(false);
    }

    QByteArray &&by=obj->getLastData();
    ui->outputLab->setText(by);
    setHighlight();
}


void Widget::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QVariant var=item->data(0,Qt::DisplayRole+1);
    currentId=var.toInt();
    std::shared_ptr<DebugObject> obj=manager->getDebugObject(currentId);
    if(obj){
        updateUI(obj);
    }
    qDebug()<<"currentId:"<<currentId;
}


void Widget::on_sendInput_clicked()
{
    std::shared_ptr<DebugObject> obj=manager->getDebugObject(currentId);
    if(obj){
        QVariant var;
        var= ui->inputFormat->currentData(Qt::UserRole);

        obj->write(ui->inputEdit->toPlainText().toUtf8(),IOFormat(var.toInt()), QHostAddress(ui->sendIP->text()),
                   ui->sendPort->text().toInt());
    }else{
        showError("发送失败 ");
    }
}


void Widget::on_clearInput_clicked()
{
    ui->inputEdit->setText("");
}


void Widget::on_clearOutput_clicked()
{
    ui->outputLab->setText("");
}


void Widget::on_query_clicked()
{
    std::shared_ptr<DebugObject> obj=manager->getDebugObject(currentId);
    if(obj){
        updateValue(obj);
    }else{
        qDebug()<<"查询失败";
    }

}


void Widget::on_setValSetting_clicked()
{
    std::shared_ptr<DebugObject> obj=manager->getDebugObject(currentId);
    if(obj){
        ValueSetting val;
        QVariant var;
        var= ui->bitType->currentData(Qt::UserRole);
        val.valueBitType=ValueBitType(var.toInt());

        var= ui->comboBoxEndian->currentData(Qt::UserRole);
        val.endianType=EndianType(var.toInt());

        var= ui->signedType->currentData(Qt::UserRole);
        val.signedType=SignedType(var.toInt());

        var= ui->bitType->currentData(Qt::UserRole);
        if(ui->lineEditOffset->text()==""){
            val.valueOffset=0;
        }else{
            val.valueOffset=ui->lineEditOffset->text().toInt();
        }
        obj->setValue(val);
        updateUI(obj);
    }else{
       showError("应用失败");
    }

}


void Widget::on_updateOutpt_clicked()
{
    std::shared_ptr<DebugObject> obj=manager->getDebugObject(currentId);
    if(obj){
        QVariant var;
        var= ui->comboBoxOutFormat->currentData(Qt::UserRole);

        obj->setOutputFormat(IOFormat(var.toInt()));
        updateUI(obj);

    }else{
        showError("应用失败");
    }
}

void Widget::createDebug(DebugSetting set)
{
    int ret=-1;
    if((ret=manager->createDebug(set))>0){
        QList<QTreeWidgetItem*> items;
        QTreeWidgetItem*item=new QTreeWidgetItem;
        item->setText(0,protocolTypeToString(set.protocolType)+QString::number(ret));
        item->setData(0,Qt::DisplayRole+1,ret);
        items<<item;
        ui->treeWidget->appendItem(nullptr,items);
    }else if(ret==-2){
        showError("暂不支持该类型\n");
    }else{
        showError("创建失败");
    }
}


void Widget::on_copyBtn_clicked()
{
    QClipboard *clip = QApplication::clipboard();
    clip->setText(ui->outputLab->toPlainText().replace(" ",""),QClipboard::Clipboard);
}

void Widget::setHighlight()
{
    QString &&by=ui->outputLab->toPlainText();
    ui->outputLab->setText(by);
    QTextCursor cursor = ui->outputLab->textCursor();
    int tmpStart=ui->highlightStart->value();
    int tmpEnd=ui->highlightEnd->value();
    std::shared_ptr<DebugObject> obj=manager->getDebugObject(currentId);
    if(obj){
        if(obj->getDebugSetting().oFormat==IOFormat::TO_HEX){
            tmpStart=3*tmpStart;
            tmpEnd=3*tmpEnd;
        }
    }
    tmpEnd=tmpStart+tmpEnd;
    tmpStart=tmpStart>by.length()?by.length():tmpStart;
    tmpEnd=tmpEnd>by.length()?by.length():tmpEnd;

    cursor.setPosition(tmpStart);
    cursor.setPosition(tmpEnd,QTextCursor::MoveMode::KeepAnchor);
    QTextCharFormat fmt;
    fmt.setForeground(QColor(Qt::red));
    cursor.mergeCharFormat(fmt);    //设置文本格式
    cursor.clearSelection(); //撤销选中
    cursor.movePosition(QTextCursor::EndOfLine);  //cursor和anchor均移至末尾
}
template <class  T>
T calcBitValue(T i,int index,int n)
{
    int leftShift=sizeof(T)*8-(index+n);
    i<<=leftShift;
    i>>=index+leftShift;
    return i;
};

void Widget::updateValue(std::shared_ptr<DebugObject> obj)
{
    QString res="";
    bool ret=obj->getValue(res);
    if(!ret){
        ui->bitValue->setText("查询失败");
        ui->lineEditValue->setText("查询失败");
        return;
    }
    ui->lineEditValue->setText(res);

    int start=ui->bitStart->value();
    int count=ui->bitCount->value();
    ValueBitType type=obj->getDebugSetting().value.valueBitType;

    if(type==ValueBitType::INT8_T){
        uint8_t v= res.toInt();
        ui->bitValue->setText(QString("%1").arg(calcBitValue(v,start,count)));
    } else   if(type==ValueBitType::INT16_T){
        uint16_t v= res.toInt();
        ui->bitValue->setText(QString("%1").arg(calcBitValue(v,start,count)));
    }else  if(type==ValueBitType::INT32_T){
        uint32_t v= res.toInt();
        ui->bitValue->setText(QString("%1").arg(calcBitValue(v,start,count)));
    }else{
        ui->bitValue->setText("不支持的类型查询");
    }
}

void Widget::showError(QString error)
{
    QMessageBox::warning(nullptr,"错误",error);
}


void Widget::on_highlightStart_valueChanged(int arg1)
{
    setHighlight();
}


void Widget::on_highlightEnd_valueChanged(int arg1)
{
    setHighlight();
}


