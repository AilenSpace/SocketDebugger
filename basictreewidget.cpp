#include "basictreewidget.h"

BasicTreeWidget::BasicTreeWidget(QWidget *parent)
    : QTreeWidget{parent}
{

}

void BasicTreeWidget::clear(QTreeWidgetItem *item)
{
    if(!item){
        int count=this->topLevelItemCount();
        for(int i=count-1;i>=0;i--){
            delete this->topLevelItem(i);
        }

    }else{
       emit deleteItemed(item->data(0,Qt::DisplayRole+1));
       delete item;
    }
}

void BasicTreeWidget::appendItem(QTreeWidgetItem *parent, QList<QTreeWidgetItem *> items)
{
    if(!parent){
        this->addTopLevelItems(items);
    }else{
        parent->addChildren(items);
    }
}
