#include "grouplist.h"

#include <QMenu>
#include <QMouseEvent>

GroupList::GroupList(int width, int height, bool hasSign, QWidget *parent) :
    MyListWidget(width,height,hasSign,parent)
{

}

void GroupList::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu popMenu(this);
    QAction action("查看资料",this);
    QAction action2("退出群聊",this);
    if(itemAt(mapFromGlobal(QCursor::pos())) != nullptr){
        popMenu.addAction(&action);
        popMenu.addAction(&action2);
    }
    connect(&action,SIGNAL(triggered()),this,SLOT(viewInformation()));
    connect(&action2,SIGNAL(triggered()),this,SLOT(delThisGroup()));
    popMenu.exec(QCursor::pos());
    event->ignore();
}

void GroupList::delThisGroup()
{
    MyWidget *currentWidget = static_cast<MyWidget*>(itemWidget(currentItem()));
    emit delGroup(currentWidget->getId());
}
