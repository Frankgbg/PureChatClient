#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include "mywidget.h"

#include <QListWidget>

class MyListWidget : public QListWidget
{
    Q_OBJECT
public:
    MyListWidget(int width, int height, bool hasSign, QWidget *parent = nullptr);
    ~MyListWidget();

    void contextMenuEvent (QContextMenuEvent * event ) override;//菜单

public:
    inline void setHasSign(const bool &hasSign) { _hasSign = hasSign;}//设置是否有签名
    inline bool getHasSign() const { return _hasSign;}//返回是否有签名
    inline bool getWidth() const { return _width;}//返回对象宽度
    inline bool getheight() const { return _height;}//返回对象高度
    void addWidget(qulonglong id, const QPixmap &image, const QString &name, const QString &sign = "");//添加对象
    void delWidget(qulonglong id);//删除对象
    void updatePixmap(qulonglong id, const QPixmap &image);//更新对象头像
    void updateName(qulonglong id, const QString &name);//更新对象昵称
    void updateSign(qulonglong id, const QString &sign);//更新对象签名

protected slots:
    void viewInformation();

private:
    QListWidgetItem *seekItem(qulonglong id);//返回id对象

private:
    int _width;
    int _height;
    bool _hasSign;

signals:
    void showInformation(MyWidget *currentWidget);
};

#endif // MYLISTWIDGET_H
