#include "createobject.h"
#include "ui_createobject.h"
#include<QVector>
#include<QIntValidator>
#include<QPalette>
#include <memory>
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
    for(int i=(int)ProtocolType::MIN+1;i<(int)ProtocolType::MAX;i++){
        ui->comboBoxProto->addItem(protocolTypeToString(ProtocolType(i)),i);
    }
    ui->srcPort->setValidator(new QIntValidator);
    ui->srcPort->setText("9060");
    ui->srcIP->setText("127.0.0.1");

    ui->destPort->setValidator(new QIntValidator);
    ui->destPort->setText("9061");
    ui->destIP->setText("127.0.0.1");
}

void CreateObject::on_create_clicked()
{
    ValueSetting val;
    val.valueBitType=ValueBitType::INT8_T;
    val.endianType=EndianType::SMALL;
    val.signedType=SignedType::Unsigned;
    val.valueOffset=0;


    AdvSetting adv;
    adv.readMode=ReadMode::ReadAll;
    adv.packageSize=val;
    adv.fixedSize=0;
    adv.packageSize=val;

    std::shared_ptr<BasicSetting>  set;


    QVariant type=ui->comboBoxProto->itemData(ui->comboBoxProto->currentIndex(),Qt::UserRole);
    ProtocolType protocolType=ProtocolType(type.toInt());
    if(protocolType==ProtocolType::UDP_CLIENT||protocolType==ProtocolType::UDP_SERVRE){
        std::shared_ptr<UdpSetting> udp=std::make_shared<UdpSetting>();
        udp->srcIp=QHostAddress(ui->srcIP->text());
        udp->srcPort=ui->srcPort->text().toInt();
        set=udp;
    }else if(protocolType==ProtocolType::TCP_CLIENT||protocolType==ProtocolType::TCP_SERVRE){
        std::shared_ptr<TcpSetting> tcp=std::make_shared<TcpSetting>();
        tcp->srcIp=QHostAddress(ui->srcIP->text());
        tcp->srcPort=ui->srcPort->text().toInt();
        tcp->destIp=QHostAddress(ui->destIP->text());
        tcp->destPort=ui->destPort->text().toInt();
        set=tcp;
    }
    set->protocolType=protocolType;
    DebugSetting setting;
    setting.oFormat=IOFormat::TO_HEX;
    setting.advSetting=adv;
    setting.value=val;
    setting.setting=set;

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
    ui->destIP->setVisible(true);
    ui->destIPLab->setVisible(true);
    ui->destPort->setVisible(true);
    ui->destPortLab->setVisible(true);
    ui->srcIP->setVisible(true);
    ui->srcIPLab->setVisible(true);
    ui->srcPort->setVisible(true);
    ui->srcPortLab->setVisible(true);
    if(type==ProtocolType::UDP_CLIENT||type==ProtocolType::TCP_SERVRE||type==ProtocolType::UDP_SERVRE){
        ui->destIP->setVisible(false);
        ui->destIPLab->setVisible(false);
        ui->destPort->setVisible(false);
        ui->destPortLab->setVisible(false);
    }
}

