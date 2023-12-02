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

QTreeWidgetItem *BasicTreeWidget::findId(int id)
{
    std::function<QTreeWidgetItem*(QTreeWidgetItem *,int)> tmpFind=[&](QTreeWidgetItem *item,int id)->QTreeWidgetItem*{
        if(item->data(0,Qt::DisplayRole+1).toInt()==id){
            return item;
        }
        for(int i=item->childCount()-1;i>=0;i--){
            QTreeWidgetItem * tmpItem=tmpFind(item->child(i),id);
            if(tmpItem)return tmpItem;
        }
        return nullptr;
    };
    int count=this->topLevelItemCount();
    for(int i=count-1;i>=0;i--){
        QTreeWidgetItem *item=tmpFind(this->topLevelItem(i),id);
        if(item)return item;
    }
    return nullptr;
}
