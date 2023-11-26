#include "createobject.h"
#include "ui_createobject.h"
#include<QVector>
#include<QIntValidator>
#include<QPalette>
CreateObject::CreateObject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateObject)
{
    ui->setupUi(this);
    initData();
}

CreateObject::~CreateObject()
{
    delete ui;
}

void CreateObject::initData()
{
    for(int i=(int)ProtocolType::MIN;i<(int)ProtocolType::MAX;i++){
        ui->comboBoxProto->addItem(protocolTypeToString(ProtocolType(i)),i);
    }
    ui->lineEditPort->setValidator(new QIntValidator);
    ui->lineEditPort->setText("9060");
    ui->lineEditIP->setText("127.0.0.1");
}

void CreateObject::on_create_clicked()
{
    ValueSetting val;
    val.valueBitType=ValueBitType::INT8_T;
    val.endianType=EndianType::SMALL;
    val.signedType=SignedType::Signed;
    val.valueOffset=0;
    DebugSetting setting;

    setting.oFormat=IOFormat::BYTE_ARRAY;

    QVariant type=ui->comboBoxProto->itemData(ui->comboBoxProto->currentIndex(),Qt::UserRole);
    setting.protocolType=ProtocolType(type.toInt());
    if(setting.protocolType==ProtocolType::UDP_CLIENT){
        setting.ip=QHostAddress("");
        setting.port=0;
    }else{
        setting.ip=QHostAddress(ui->lineEditIP->text());
        setting.port=ui->lineEditPort->text().toUInt();
    }

    HeadSetting head;
    head.hasPacketHead=false;
    head.fullPacketSize=true;//是否是包含 包头的大小
    head.packageSize=val;//形容包大小

    setting.head=head;
    setting.value=val;

    emit createDebug(setting);
    this->close();
}


void CreateObject::on_cancel_clicked()
{
    this->close();
}


void CreateObject::on_comboBoxProto_currentIndexChanged(int index)
{

}


void CreateObject::on_comboBoxProto_currentTextChanged(const QString &arg1)
{
    QVariant var=ui->comboBoxProto->currentData(Qt::UserRole);
    ProtocolType type=ProtocolType(var.toInt());
    if(type==ProtocolType::UDP_CLIENT){
        ui->label_2->setVisible(false);
        ui->label->setVisible(false);
        ui->lineEditIP->setVisible(false);
        ui->lineEditPort->setVisible(false);
    }else{
        ui->label_2->setVisible(true);
        ui->label->setVisible(true);
        ui->lineEditIP->setVisible(true);
        ui->lineEditPort->setVisible(true);
    }

}

