#include "createvaluequery.h"
#include "ui_createvaluequery.h"
#include<QDoubleValidator>
CreateValueQuery::CreateValueQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateValueQuery)
{
    ui->setupUi(this);
    initData();
}

CreateValueQuery::~CreateValueQuery()
{
    delete ui;
}

void CreateValueQuery::initData()
{

    //value
    for(int i=(int)EndianType::MIN+1;i<(int)EndianType::MAX;i++){
        ui->endian->addItem(endianTypeToString(EndianType(i)),i);
    }
    for(int i=(int)ValueBitType::MIN+1;i<(int)ValueBitType::MAX;i++){
        ui->bitType->addItem(valueBitTypeToString(ValueBitType(i)),i);
    }

    for(int i=(int)SignedType::MIN+1;i<(int)SignedType::MAX;i++){
        ui->signedType->addItem(signedTypeToString(SignedType(i)),i);
    }
    ui->indexEdit->setValidator(new QDoubleValidator);
}

void CreateValueQuery::on_backspaceBtn_clicked()
{
    emit backsapce();
}


void CreateValueQuery::on_pushButton_clicked()
{
    ValueSetting set;
    set.alias=ui->aliasEdit->text();
    set.index=ui->indexEdit->text().toDouble();
    QVariant var;
    var= ui->endian->currentData(Qt::UserRole);
    set.endianType=EndianType(var.toInt());

    var= ui->bitType->currentData(Qt::UserRole);
    set.valueBitType=ValueBitType(var.toInt());

    var= ui->signedType->currentData(Qt::UserRole);
    set.signedType=SignedType(var.toInt());

    set.valueOffset=ui->valOffset->text().toInt();
    set.bitStart=ui->bitStart->value();
    set.bitLength=ui->bitLength->value();
    emit sure(set);
}

