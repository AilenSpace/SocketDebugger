#include "recordwidget.h"
#include "ui_recordwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QClipboard>
#include <QScrollBar>
#include "object/debugobject.h"
RecordItem::RecordItem(QWidget *parent):QWidget(parent)
{
    frameId=new QLabel;
    frameId->setEnabled(false);
    byEdit=new QTextEdit;
    byEdit->setEnabled(false);

    copyBtn=new QPushButton;
    copyBtn->setText("复制");
    showFormat=new QComboBox;

    for(int i=(int)IOFormat::MIN+1;i<(int)IOFormat::MAX;i++){
        showFormat->addItem(IOFormatToString(IOFormat(i)),i);
    }

    typedef void (QComboBox::*ActivatedTFunc)(int);
    ActivatedTFunc sig=&QComboBox::activated;
    connect(showFormat,sig,this,[=](int index){
        QVariant var;
        var= showFormat->currentData(Qt::UserRole);
        byEdit->setText(DebugObject::getFomateData(data.by,IOFormat(var.toInt())));
    });

    showFormat->setCurrentText(IOFormatToString(IOFormat::TO_HEX));

    QVBoxLayout *lay=new QVBoxLayout;
    lay->addWidget(frameId);
    lay->addWidget(byEdit);
    QHBoxLayout *hlay=new QHBoxLayout;
    hlay->addStretch();
    hlay->addWidget(showFormat);
    hlay->addWidget(copyBtn);
    lay->addLayout(hlay);
    this->setLayout(lay);

    connect(copyBtn,&QPushButton::clicked,this,[=](){
        QClipboard *clip = QApplication::clipboard();
        clip->setText(byEdit->toPlainText(),QClipboard::Clipboard);
    });
}

RecordItem::~RecordItem()
{

}

void RecordItem::setData(DebugObject::DataT data)
{
    this->data=data;
    frameId->setText(QString("序号:%1").arg(data.frameId));
    QVariant var;
    var= showFormat->currentData(Qt::UserRole);
    byEdit->setText(DebugObject::getFomateData(data.by,IOFormat(var.toInt())));
}

RecordWidget::RecordWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecordWidget)
{
    ui->setupUi(this);
    sizeH=300;
}

RecordWidget::~RecordWidget()
{
    delete ui;
}

void RecordWidget::setData(QList<DebugObject::DataT> datas)
{
    ui->listWidget->clear();
    for(int i=0;i<datas.size();i++){
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget,0);
        RecordItem* widget = new RecordItem(ui->listWidget);
        widget->setData(datas[i]);
        int w=this->width()-ui->listWidget->horizontalScrollBar()->width();
        w=w<0?100:w;
        item->setSizeHint(QSize(0,sizeH));
        widget->show();
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);
    }
}

void RecordWidget::setItemHeight(int h)
{
    sizeH=h;
}

