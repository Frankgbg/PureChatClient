#include "mylistwidget.h"
#include "mywidget.h"

#include <QMenu>
#include <QMouseEvent>
#include <QDebug>

MyListWidget::MyListWidget(int width, int height, bool hasSign, QWidget *parent)
    :QListWidget(parent)
{
    _width = width;
    _height = height;
    _hasSign = hasSign;
    this->setStyleSheet("QListWidget{border:none;background:rgb(235,242,249);}\n"
                        "QListWidget::item:hover{background-color:#FCF0C1;}");//鼠标覆盖颜色
}

MyListWidget::~MyListWidget()
{

}

void MyListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu popMenu(this);
    QAction action("查看资料",this);
    if(itemAt(mapFromGlobal(QCursor::pos())) != nullptr){
        popMenu.addAction(&action);
    }
    connect(&action,SIGNAL(triggered()),this,SLOT(viewInformation()));
    popMenu.exec(QCursor::pos());
    event->ignore();
}

void MyListWidget::addWidget(qulonglong id, const QPixmap &image, const QString &name, const QString &sign)
{
    QListWidgetItem *listItem = seekItem(id);
    if(listItem != nullptr) {
        MyWidget *myWidget = static_cast<MyWidget*>(this->itemWidget(listItem));
        myWidget->setPixmap(image);
        myWidget->setName(name);
        myWidget->setSign(sign);
        return ;
    }
    listItem = new QListWidgetItem();
    listItem->setSizeHint(QSize(0,_height));
    this->addItem(listItem);
    MyWidget *myWidget = new MyWidget(_width,_height);
    myWidget->setHasSign(_hasSign);
    myWidget->init(id,image,name,sign);
    this->setItemWidget(listItem, myWidget);
}

void MyListWidget::delWidget(qulonglong id)
{
    QListWidgetItem *_item = seekItem(id);
    if(_item != nullptr) {
        delete this->itemWidget(_item);
        QListWidgetItem *it = this->takeItem(row(_item));
        delete it;
    }
}

void MyListWidget::updatePixmap(qulonglong id, const QPixmap &image)
{
    QListWidgetItem *_item = seekItem(id);
    if(_item != nullptr){
        MyWidget *myWidget = static_cast<MyWidget*>(this->itemWidget(_item));
        myWidget->setPixmap(image);
    }
}

void MyListWidget::updateName(qulonglong id, const QString &name)
{
    QListWidgetItem *_item = seekItem(id);
    if(_item != nullptr){
        MyWidget *myWidget = static_cast<MyWidget*>(this->itemWidget(_item));
        myWidget->setName(name);
    }
}

void MyListWidget::updateSign(qulonglong id, const QString &sign)
{
    if(!_hasSign) return ;
    QListWidgetItem *_item = seekItem(id);
    if(_item != nullptr){
        MyWidget *myWidget = static_cast<MyWidget*>(this->itemWidget(_item));
        myWidget->setSign(sign);
    }
}

void MyListWidget::viewInformation()
{
    MyWidget *currentWidget = static_cast<MyWidget*>(itemWidget(currentItem()));
    emit showInformation(currentWidget);
}

QListWidgetItem *MyListWidget::seekItem(qulonglong id)
{
    for(int i = 0;i < this->count();++i){
        QListWidgetItem *_item = this->item(i);
        MyWidget *myWidget = static_cast<MyWidget*>(this->itemWidget(_item));
        if(myWidget->getId() == id){
            return _item;
        }
    }
    return nullptr;
}

