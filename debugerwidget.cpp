#include "debugerwidget.h"
#include "ui_debugerwidget.h"
#include <QClipboard>
#include <QMessageBox>
DebugerWidget::DebugerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DebugerWidget)
{
    ui->setupUi(this);
    initData();
}

int DebugerWidget::getId()
{
    if(!object)return -1;
    return object->getId();
}

DebugerWidget::~DebugerWidget()
{
    delete ui;
}

void DebugerWidget::setDebugObject(std::shared_ptr<DebugObject> obj)
{
    if(object)
        disconnect(this->object.get(),&DebugObject::newData,this,&DebugerWidget::onNewData);
    this->object=obj;
    connect(this->object.get(),&DebugObject::newData,this,&DebugerWidget::onNewData);
    updateUI(object);
}

void DebugerWidget::initData()
{

    ui->inputInfo->setText("");
    ui->outputInfo->setText("");

    //info
    for(int i=(int)ProtocolType::MIN+1;i<(int)ProtocolType::MAX;i++){
        ui->infoProto2->addItem(protocolTypeToString(ProtocolType(i)),i);
    }
    //value
    for(int i=(int)ValueBitType::MIN+1;i<(int)ValueBitType::MAX;i++){
        ui->bitType->addItem(valueBitTypeToString(ValueBitType(i)),i);
    }
    for(int i=(int)EndianType::MIN+1;i<(int)EndianType::MAX;i++){
        ui->endian->addItem(endianTypeToString(EndianType(i)),i);
    }
    for(int i=(int)SignedType::MIN+1;i<(int)SignedType::MAX;i++){
        ui->signedType->addItem(signedTypeToString(SignedType(i)),i);
    }



    //basic
    for(int i=(int)AcquisitionMode::MIN+1;i<(int)AcquisitionMode::MAX;i++){
        ui->acquisitionMode->addItem(acquisitionModeToString(AcquisitionMode(i)),i);
    }

    //adv
    for(int i=(int)ReadMode::MIN+1;i<(int)ReadMode::MAX;i++){
        ui->readMode->addItem(readModeToString(ReadMode(i)),i);
    }

    for(int i=(int)ValueBitType::MIN+1;i<(int)ValueBitType::MAX;i++){
        ui->advBitType->addItem(valueBitTypeToString(ValueBitType(i)),i);
    }
    for(int i=(int)EndianType::MIN;i<(int)EndianType::MAX;i++){
        ui->advEndian->addItem(endianTypeToString(EndianType(i)),i);
    }
    for(int i=(int)SignedType::MIN+1;i<(int)SignedType::MAX;i++){
        ui->advSignedType->addItem(signedTypeToString(SignedType(i)),i);
    }

    //上
    ui->startBtn->setEnabled(false);
    ui->stopBtn->setEnabled(false);

    //中
    for(int i=(int)IOFormat::MIN+1;i<(int)IOFormat::MAX;i++){
        ui->outputFormat->addItem(IOFormatToString(IOFormat(i)),i);
    }


    //下面
    for(int i=(int)IOFormat::MIN+1;i<(int)IOFormat::MAX;i++){
        ui->inputFormat->addItem(IOFormatToString(IOFormat(i)),i);
    }
    ui->inputFormat->setCurrentText(IOFormatToString(IOFormat::FROM_HEX));

    ui->inputFormat->setCurrentText(IOFormatToString(IOFormat::BYTE_ARRAY));
    ui->inputInfo->setText("1111");
    ui->sendIP->setText("127.0.0.1");
    ui->sendPort->setText("9060");
}

void DebugerWidget::updateUI(std::shared_ptr<DebugObject> obj)
{   
    DebugSetting set=  obj->getSetting();
    updateInfo(set);
    updateValueQuery(set);
    updateSetting(set);
    updateAdvSetting(set);

    QByteArray &&by=obj->getLastData();
    ui->outputInfo->setText(by);
    refreshHighlight();
    updateValue(obj);

    ui->startBtn->setEnabled(!obj->isOpen());
    ui->stopBtn->setEnabled(obj->isOpen());

    if(set.setting->protocolType==ProtocolType::UDP_CLIENT){
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
}

void DebugerWidget::updateInfo(const DebugSetting &set)
{
    ProtocolType type=set.setting->protocolType;
    ui->infoProto2->setCurrentText(protocolTypeToString(type));
    if(type==ProtocolType::TCP_CLIENT||type==ProtocolType::TCP_SERVRE){
        std::shared_ptr<TcpSetting> tcpSetting= std::dynamic_pointer_cast<TcpSetting>(set.setting);

        ui->infoDestIP2->setText(tcpSetting->destIp.toString());
        ui->infoDestPort2->setText(QString::number(tcpSetting->destPort));
        ui->infoSrcIP2->setText(tcpSetting->srcIp.toString());
        ui->infoSrcPort2->setText(QString::number(tcpSetting->srcPort));

    }else if(type==ProtocolType::UDP_CLIENT||type==ProtocolType::UDP_SERVRE){
        std::shared_ptr<UdpSetting> udpSetting= std::dynamic_pointer_cast<UdpSetting>(set.setting);
        ui->infoDestIP2->setText("");
        ui->infoDestPort2->setText("");
        ui->infoSrcIP2->setText(udpSetting->srcIp.toString());
        ui->infoSrcPort2->setText(QString::number(udpSetting->srcPort));
    }

}

void DebugerWidget::updateValueQuery(const DebugSetting &set)
{
    ui->bitType->setCurrentText(valueBitTypeToString(set.value.valueBitType));
    ui->endian->setCurrentText(endianTypeToString(set.value.endianType));
    ui->signedType->setCurrentText(signedTypeToString(set.value.signedType));
    ui->valOffset->setText(QString::number(set.value.valueOffset));
}

void DebugerWidget::updateSetting(const DebugSetting &set)
{
    ui->acquisitionMode->setCurrentText(acquisitionModeToString(set.acquisitionMode));
}

void DebugerWidget::updateAdvSetting(const DebugSetting &set)
{

    ui->advBitType->setCurrentText(valueBitTypeToString(set.advSetting.packageSize.valueBitType));
    ui->advEndian->setCurrentText(endianTypeToString(set.advSetting.packageSize.endianType));
    ui->advSignedType->setCurrentText(signedTypeToString(set.advSetting.packageSize.signedType));
    ui->advValueOffset->setText(QString::number(set.advSetting.packageSize.valueOffset));

    ui->fixedSize->setText(QString::number(set.advSetting.fixedSize));
    ui->readMode->setCurrentText(readModeToString(set.advSetting.readMode));
}

void DebugerWidget::refreshHighlight()
{
    if(!object)
    {
        emit showError("debuger object not setting");
        return;
    }
    QString &&by=ui->outputInfo->toPlainText();
    ui->outputInfo->setText(by);
    QTextCursor cursor = ui->outputInfo->textCursor();
    int tmpStart=ui->highlightStart->value();
    int tmpEnd=ui->highlightLength->value();


    if(object->getSetting().oFormat==IOFormat::TO_HEX){
        tmpStart=3*tmpStart;
        tmpEnd=3*tmpEnd;
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

void DebugerWidget::updateValue(std::shared_ptr<DebugObject> obj)
{
    QString res="";
    bool ret=obj->getValue(res);
    if(!ret){
        ui->value->setText(tr("查询失败 "));
        ui->bitValue->setText(tr("查询失败 "));
        return;
    }
    ui->value->setText(res);

    int start=ui->bitStart->value();
    int count=ui->bitLength->value();
    ValueBitType type=obj->getSetting().value.valueBitType;

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

void DebugerWidget::on_setValSetting_clicked()
{
    if(!object)
    {
        emit showError("debuger object not setting");
        return;
    }
    ValueSetting val;
    QVariant var;
    var= ui->bitType->currentData(Qt::UserRole);
    val.valueBitType=ValueBitType(var.toInt());

    var= ui->endian->currentData(Qt::UserRole);
    val.endianType=EndianType(var.toInt());

    var= ui->signedType->currentData(Qt::UserRole);
    val.signedType=SignedType(var.toInt());

    if(ui->valOffset->text()==""){
        val.valueOffset=0;
    }else{
        val.valueOffset=ui->valOffset->text().toInt();
    }
    object->setValue(val);
    updateValue(object);
}


void DebugerWidget::on_query_clicked()
{

}


void DebugerWidget::on_copyBtn_clicked()
{
    QClipboard *clip = QApplication::clipboard();
    clip->setText(ui->outputInfo->toPlainText().replace(" ",""),QClipboard::Clipboard);
}


void DebugerWidget::on_clearOutputBtn_clicked()
{
    ui->outputInfo->setText("");
    if(!object)object->clear();;

}


void DebugerWidget::on_highlightStart_textChanged(const QString &arg1)
{
    refreshHighlight();
}


void DebugerWidget::on_highlightLength_textChanged(const QString &arg1)
{
    refreshHighlight();
}


void DebugerWidget::on_sendBtn_clicked()
{
    if(!object)
    {
        emit showError("debuger object not setting");
        return;
    }
    QVariant var;
    var= ui->inputFormat->currentData(Qt::UserRole);
    bool ret=object->wirteData(ui->inputInfo->toPlainText().toUtf8(),IOFormat(var.toInt()), QHostAddress(ui->sendIP->text()),
                               ui->sendPort->text().toInt());
    if(!ret){
        emit showError("发送失败 ");
    }
}


void DebugerWidget::on_clearInputBtn_clicked()
{
    ui->inputInfo->setText("");
}


void DebugerWidget::on_updateAdvSet_clicked()
{
    if(!object)
    {
        emit showError("debuger object not setting");
        return;
    }
    ValueSetting val;
    QVariant var;
    var= ui->advBitType->currentData(Qt::UserRole);
    val.valueBitType=ValueBitType(var.toInt());

    var= ui->advEndian->currentData(Qt::UserRole);
    val.endianType=EndianType(var.toInt());

    var= ui->advSignedType->currentData(Qt::UserRole);
    val.signedType=SignedType(var.toInt());

    if(ui->advValueOffset->text()==""){
        val.valueOffset=0;
    }else{
        val.valueOffset=ui->advValueOffset->text().toInt();
    }
    AdvSetting setting;
    setting.packageSize=val;
    setting.fixedSize=ui->fixedSize->text().toInt();
    setting.readMode=ReadMode(ui->readMode->currentData(Qt::UserRole).toInt());

    object->setAdvSetting(setting);
}


void DebugerWidget::on_startBtn_clicked()
{
    if(!object)
    {
        emit showError("debuger object not setting");
        return;
    }
    if(object->start()){
        ui->startBtn->setEnabled(!object->isOpen());
        ui->stopBtn->setEnabled(object->isOpen());
        return ;
    }
}


void DebugerWidget::on_stopBtn_clicked()
{
    if(!object)
    {
        emit showError("debuger object not setting");
        return;
    }
    if(object->stop()){
        ui->startBtn->setEnabled(!object->isOpen());
        ui->stopBtn->setEnabled(object->isOpen());
        return ;
    }
}


void DebugerWidget::on_acquisitionMode_activated(int index)
{
    if(!object)
    {
        emit showError("debuger object not setting");
        return;
    }

    QVariant var;
    var= ui->acquisitionMode->currentData(Qt::UserRole);
    object->setAcquisitionMode(AcquisitionMode(var.toInt()));

}


void DebugerWidget::on_advBitType_activated(int index)
{
    QVariant var;
    var= ui->advBitType->currentData(Qt::UserRole);
    ValueBitType type=ValueBitType(var.toInt());
    if(type==ValueBitType::FLOAT32_T||type==ValueBitType::DOUBLE64_T){
        ui->advSignedType->setCurrentText(signedTypeToString(SignedType::Signed));
        ui->advSignedType->setEnabled(false);
    }else{
        ui->advSignedType->setEnabled(true);
    }
}


void DebugerWidget::on_bitType_activated(int index)
{
    QVariant var;
    var= ui->bitType->currentData(Qt::UserRole);
    ValueBitType type=ValueBitType(var.toInt());
    if(type==ValueBitType::FLOAT32_T||type==ValueBitType::DOUBLE64_T){
        ui->signedType->setCurrentText(signedTypeToString(SignedType::Signed));
        ui->signedType->setEnabled(false);
    }else{
        ui->signedType->setEnabled(true);
    }
}

void DebugerWidget::onNewData(int id, QByteArray by)
{
    ui->outputInfo->setText(by);
    refreshHighlight();
    updateValue(object);
}


void DebugerWidget::on_outputFormat_activated(int index)
{
    if(!object)
    {
        emit showError("debuger object not setting");
        return;
    }
    QVariant var;
    var= ui->outputFormat->currentData(Qt::UserRole);
    object->setOutputFormat(IOFormat(var.toInt()));
    ui->outputInfo->setText(object->getLastData());
}

void DebugerWidget::on_historyRecv_clicked()
{
    if(!object)
    {
        emit showError("debuger object not setting");
        return;
    }
    ui->records->setData(object->getRecvRecords());
}


void DebugerWidget::on_historySend_clicked()
{
    if(!object)
    {
        emit showError("debuger object not setting");
        return;
    }
    ui->records->setData(object->getSendRecords());
}

