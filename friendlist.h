#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include "mylistwidget.h"

class FriendList : public MyListWidget
{
    Q_OBJECT
public:
    FriendList(int width, int height, bool hasSign, QWidget *parent = nullptr);

    void contextMenuEvent (QContextMenuEvent * event ) override;//菜单

protected slots:
    void delThisFriend();

signals:
    void delFriend(qulonglong friendId);
};

#endif // FRIENDLIST_H
