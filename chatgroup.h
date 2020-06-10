#ifndef CHATGROUP_H
#define CHATGROUP_H

#include "emojs.h"
#include "mylistwidget.h"
#include "mywidget.h"

#include <QStackedLayout>
#include <QWidget>

namespace Ui {
class ChatGroup;
}

class ChatGroup : public QWidget
{
    Q_OBJECT
public:
    ChatGroup(qulonglong groupId, const QPixmap &groupImage, const QString &groupName, QWidget *parent = nullptr);
    ~ChatGroup();

    void mousePressEvent(QMouseEvent *event) override;//鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;//鼠标移动事件
    bool eventFilter(QObject *object, QEvent *e) override;//事件过滤器

public:
    inline qulonglong getGroupId() const { return _groupId;}//返回此窗口群Id
    void append(qulonglong sendId, QString sendName, int type, const QString &msg);//接收消息
    void addFriend(qulonglong friendId, const QPixmap &friendImage, const QString &friendName, const QString &friendSign);//添加群成员

public slots:
    void showInformation(MyWidget *currend);//群成员查看资料卡

private slots:
    void on_pushButton_clicked();//关闭槽
    void on_pushButton_2_clicked();//发送槽
    void sendEmoj(QString emoj);//发送表情槽
    void showChatFriend(QListWidgetItem *chatFriend);//群成员打开聊天窗口

private:
    Ui::ChatGroup *ui;
    QPoint offSet;//鼠标和窗口位置偏移量
    qulonglong _groupId;//群Id
    QString _groupName;//群昵称
    Emojs *emojs;//表情
    MyListWidget *friends;//群内好友

signals:
    void send(int type,qulonglong groupId, const QString &groupName, const QString &msg);//发送消息信号
    void removeThis(qulonglong groupId);//由homepage delete此窗口
    void toHomeShowChatFriend(MyWidget *myWidget);
    void toHomeShowInformation(bool editable, MyWidget *currentWidget);//群成员查看资料卡发给homepage
};

#endif // CHATGROUP_H
