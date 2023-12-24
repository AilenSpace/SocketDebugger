#include "toolwidget.h"
#include <QGridLayout>

ToolWidget::ToolWidget(QWidget *parent)
    : QWidget{parent}
{
    toolBar=new QToolBar;
    QGridLayout *lay=new QGridLayout;
    lay->addWidget(toolBar);
    lay->setContentsMargins(0,0,0,0);
    this->setLayout(lay);
    this->setContentsMargins(0,0,0,0);
}

void ToolWidget::addAction(const QString &title, std::function<void ()> func)
{
    toolBar->addAction(title,func);
}
