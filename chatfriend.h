#ifndef CHATFRIEND_H
#define CHATFRIEND_H

#include "emojs.h"

#include <QWidget>

namespace Ui {
class ChatFriend;
}

class ChatFriend : public QWidget
{
    Q_OBJECT
public:
    ChatFriend(qulonglong friendId, const QPixmap &friendImage, const QString &friendName, QWidget *parent = nullptr);
    ~ChatFriend();

    void mousePressEvent(QMouseEvent *event) override;//鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;//鼠标移动事件
    bool eventFilter(QObject *object, QEvent *e) override;//事件过滤器

public:
    inline qulonglong getFriendId() const { return _friendId;};//返回此窗口好友Id
    void append(int type, const QString &msg);//接收消息

private slots:
    void on_pushButton_clicked();//关闭槽
    void on_pushButton_2_clicked();//发送槽
    void sendEmoj(QString emoj);//发送表情槽

private:
    Ui::ChatFriend *ui;
    QPoint offSet;//鼠标和窗口位置偏移量
    qulonglong _friendId;//好友Id
    QString _friendName;//好友昵称
    Emojs *emojs;//表情

signals:
    void send(int type, qulonglong friendId, const QString &friendName, const QString &msg);//发送消息信号
    void removeThis(qulonglong friendId);//由homepage 移除此窗口
};

#endif // CHATFRIEND_H
