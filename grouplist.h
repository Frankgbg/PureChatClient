#ifndef GROUPLIST_H
#define GROUPLIST_H

#include "mylistwidget.h"

class GroupList : public MyListWidget
{
    Q_OBJECT
public:
    GroupList(int width, int height, bool hasSign, QWidget *parent = nullptr);

    void contextMenuEvent (QContextMenuEvent * event ) override;//菜单

protected slots:
    void delThisGroup();

signals:
    void delGroup(qulonglong friendId);
};

#endif // GROUPLIST_H
