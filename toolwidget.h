#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>
#include <QToolBar>
#include<functional>
class ToolWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ToolWidget(QWidget *parent = nullptr);
    void addAction(const QString &title,std::function<void()> func);
signals:

private:
    QToolBar*toolBar;
};

#endif // TOOLWIDGET_H
