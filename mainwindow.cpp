#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "debugwidget.h"
#include "planwidget.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    qRegisterMetaType<ValueSetting>("ValueSetting");
    qRegisterMetaType<Plan>("Plan");
    qRegisterMetaType<DebugSetting>("DebugSetting");
    qRegisterMetaType<std::shared_ptr<DebugObject>>("std::shared_ptr<DebugObject>");
    initData();
    initStyle();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initData()
{
    resize(1400,1000);
    PlanWidget*wid1=PlanWidget::instance();
    DebugWidget*wid2=new DebugWidget;
    ui->tabWidget->addTab(wid1,"方案");
    ui->tabWidget->addTab(wid2,"调试");
}

void MainWindow::initStyle()
{

    this->setObjectName("boardFarme");
    QString style=QString("QWidget{background-color :rgb(224,251,255);font-size:12pt}"
                          "QWidget#boardFarme,QLineEdit,QTextEdit#inputEdit,QTextEdit#outputLab,QTabWidget::pane,QComboBox"
                          "{border:1px solid rgb(192,192,192); border-radius:2px}"
                          "QSplitter::handle{border: 1px solid rgb(192,192,192);width:1px}"
                          "QPushButton{background-color :rgb(230,240,234);border-radius:6px}"
                          "QPushButton::pressed{background-color :rgb(200,233,255);}"
                          "QPushButton::hover{background-color :rgb(146,240,234);}"
                          "QTreeWidget{font-size:8pt;}"
                          "QTabWidget QTabBar::tab{background-color :rgb(206,230,234);}"
                          "QTabWidget QTabBar::tab:selected{background-color :rgb(230,240,234);}"
                          );
    this->setStyleSheet(style);

}
