#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "chatfriend.h"
#include "chatgroup.h"
#include "creategroup.h"
#include "friendlist.h"
#include "grouplist.h"
#include "information.h"
#include "owninformation.h"
#include "search.h"
#include "socket.h"
#include "socketthread.h"

#include <QPropertyAnimation>
#include <QStackedLayout>
#include <QSystemTrayIcon>

namespace Ui {
class Homepage;
}

class Homepage : public QWidget
{
    Q_OBJECT
public:
    explicit Homepage(qulonglong id, QString password, Socket *sock, QWidget *parent = nullptr);
    ~Homepage();

    void mousePressEvent(QMouseEvent *event) override;//鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *event) override;//鼠标释放事件
    void mouseMoveEvent(QMouseEvent *event) override;//鼠标移动事件
    void enterEvent(QEvent *event) override;//鼠标进入事件
    void leaveEvent(QEvent *event) override;//鼠标离开事件
    void focusOutEvent(QFocusEvent *event) override;//失去焦点
    bool eventFilter(QObject *object, QEvent *e) override;//事件过滤器
    void paintEvent(QPaintEvent *event) override;//画周围x圈

public slots:
    void onSysTrayIconActived(QSystemTrayIcon::ActivationReason reason);//任务栏图标活动槽

    void delFriend(qulonglong friendId);
    void delGroup(qulonglong groupId);

    void showChatFriend(QListWidgetItem *chatFriend);//显示好友聊天窗口
    void showChatFriend(MyWidget *myWidget);//重载
    void showChatGroup(QListWidgetItem *chatGroup);//显示群聊天窗口
    void showInformation(MyWidget *currentWidget);//显示资料卡
    void showInformation(bool editabled, MyWidget *currentWidget);//重载

    void removeChatFriend(qulonglong friendId);//删除好友聊天窗口
    void removeChatGroup(qulonglong groupId);//删除群聊窗口
    void removeInformation(bool editabled, qulonglong id);//删除资料卡editabled=true为群资料卡

    void sendMsg(int type, qulonglong id, const QString &name, const QString &msg);//发送好友或群聊天内容

    void updateInformation(int type, qulonglong id, const QString &information);//资料卡更改信息槽
    void updateImage(int type, qulonglong id, const QPixmap &newImage, const QString &imageType);//资料卡更改信息槽
    void updateInformationSuccessful(int type, qulonglong id, const QString &information);//更改信息成功
    void updateImageSuccessful(int type, qulonglong id, const QPixmap &newImage);//更改信息成功

    void updateSearch(int type, qulonglong id);//Search槽
    void createGroup(QString groupName);

    void connection();//重连上
    void disconnection();//socket断开连接
    void socketUpdate(int type, QString information, qulonglong groupId = 0);//传送数据
    void socketFriend(int type, qulonglong friendId, const QString &friendName,
                      const QString &friendSign, const QPixmap &friendImage, qulonglong groupId = 0);//传送friend
    void socketGroup(int type, qulonglong groupId,
                     const QString &groupName, const QPixmap &groupImage);//传送group
    void socketChat(int type, qulonglong sendId,
                    QString sendName, const QString &msg,
                    qulonglong groupId = 0, QString groupName = "");//传送聊天信息

private slots:
    void on_pushButton_clicked();//查找按钮
    void on_pushButton_2_clicked();//我的资料
    void on_pushButton_3_clicked();//创建群聊

    void quit();//退出
    void offline();//离线
    void online();//在线

private:
    ChatFriend *existChatFriend(qulonglong friendId);//存在好友聊天窗口返回,否则返回nullptr
    ChatGroup *existChatGroup(qulonglong groupId);//存在群聊天窗口返回,否则返回nullptr
    Information *existFriendInformation(qulonglong friendId);//存在好友资料卡返回，否则返回nullptr
    Information *existGroupInformation(qulonglong groupId);//存在群资料卡返回，否则返回nullptr

    ChatFriend *newChatFriend(qulonglong friendId, const QPixmap &friendImage, const QString &friendName);//新建好友窗口
    ChatGroup *newChatGroup(qulonglong groupId, const QPixmap &groupImage, const QString &groupName);//新建群窗口
    Information *newInformation(qulonglong id, const QPixmap &image, const QString &name,
                                   const QString &sign, bool editabled);//新建资料卡
    void imageProcessing(QPixmap &result, const QPixmap &pixmap);
    void toping(QWidget *object);//置顶窗口
    void tips(const QString &msg, QPoint pos = QPoint(900,500));//对话框提示

    void connectSocketSignals(Socket *sock);

private:
    Ui::Homepage *ui;
    int desktopHeight;//窗口高度
    QPoint offSet;//鼠标和窗口位置偏移量
    QPropertyAnimation *animation;//隐藏动画
    QSystemTrayIcon *trayIcon;//任务栏图标

    QPixmap ownImage;//个人头像
    qulonglong ownId;//个人Id
    QString ownPassword;//个人密码,修改密码,重登用
    QString ownName;//个人昵称
    QString ownSign;//个人签名

    Socket *socket;//TcpSocket
    SocketThread *socketThread;//socket断线重连

    QStackedLayout *qstacklayout;//三页页面布局
    FriendList *friends;//好友列表
    GroupList *groups;//群列表
    QHash<qulonglong, ChatFriend*> *chatFriend;//好友聊天窗口
    QHash<qulonglong, ChatGroup*> *chatGroup;//群聊天窗口
    QHash<qulonglong, Information*> *friendInformation;//好友资料
    QHash<qulonglong, Information*> *groupInformation;//群资料

    OwnInformation *ownInformation;//我的资料
    Search *search;//查找好友或群
    CreateGroup *create;//创建群聊

signals:
    void toLoginQuit();//退出PureChat
};

#endif // HOMEPAGE_H
