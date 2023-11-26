#ifndef BASICTREEWIDGET_H
#define BASICTREEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <memory>
class BasicTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit BasicTreeWidget(QWidget *parent = nullptr);
    void clear(QTreeWidgetItem *item);
    void appendItem(QTreeWidgetItem *parent,QList<QTreeWidgetItem*> items);
    QTreeWidgetItem *findId(int id);
signals:
    void deleteItemed(QVariant var);

};

#endif // BASICTREEWIDGET_H
