#include "friendlist.h"

#include <QMenu>
#include <QMouseEvent>

FriendList::FriendList(int width, int height, bool hasSign, QWidget *parent) :
    MyListWidget(width,height,hasSign,parent)
{

}

void FriendList::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu popMenu(this);
    QAction action("查看资料",this);
    QAction action2("删除好友",this);
    if(itemAt(mapFromGlobal(QCursor::pos())) != nullptr){
        popMenu.addAction(&action);
        popMenu.addAction(&action2);
    }
    connect(&action,SIGNAL(triggered()),this,SLOT(viewInformation()));
    connect(&action2,SIGNAL(triggered()),this,SLOT(delThisFriend()));
    popMenu.exec(QCursor::pos());
    event->ignore();
}

void FriendList::delThisFriend()
{
    MyWidget *currentWidget = static_cast<MyWidget*>(itemWidget(currentItem()));
    emit delFriend(currentWidget->getId());
}
