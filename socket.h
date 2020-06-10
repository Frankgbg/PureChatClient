#ifndef SOCKET_H
#define SOCKET_H

#include <QTcpSocket>

class Socket : public QTcpSocket
{
    Q_OBJECT
public:
    Socket(QTcpSocket *parent = nullptr);
    ~Socket();

public:
    void signUp(const QString &ownName, const QString &ownPassword);//注册
    void login(qulonglong ownId, const QString &ownPassword);//申请登录

    void updateOwnPassword(qulonglong ownId, const QString &oldPassword, const QString &newPassword);//修改个人密码
    void updateOwnName(qulonglong ownId, const QString &newName);//修改个人昵称
    void updateOwnSign(qulonglong ownId, const QString &newSign);//修改个人签名
    void updateOwnImage(qulonglong ownId, const QPixmap &newImage, const QString &imageType);//修改个人头像
    void updateGroupName(qulonglong groupId, const QString &newName);//修改群昵称
    void updateGroupImage(qulonglong groupId, const QPixmap &newImage, const QString &imageType);//修改群头像

    void searchFriend(qulonglong friendId);//查找好友
    void addFriend(qulonglong requestId ,qulonglong friendId);//添加好友
    void delFriend(qulonglong friendId);//删除好友

    void createGroup(const QString &groupName);//创建群聊
    void searchGroup(qulonglong groupId);//查找群
    void addGroup(qulonglong requestId ,qulonglong groupId);//添加群
    void delGroup(qulonglong groupId);//删除群

    void getFriends();//获取好友列表
    void getGroups();//获取群列表
    void getFriendsOfGroup(qulonglong group);//获取群内好友列表

    void chatFriend(qulonglong friendId, const QString &friendName, const QString &msg);//好友聊天
    void chatGroup(qulonglong groupId, const QString &groupName, const QString &msg);//群内聊天

    void sendData(const QByteArray &data);//发送数据

public slots:
    void receiveData();//接收数据槽

private:
    void receiveSignUp(const QJsonObject &json);//接收注册信息
    void receiveLogin(const QJsonObject &json);//接收登录信息

    void receiveUpdateOwnPassword(const QJsonObject &json);//修改个人密码
    void receiveUpdateOwnName(const QJsonObject &json);//修改个人昵称
    void receiveUpdateOwnSign(const QJsonObject &json);//修改个人签名
    void receiveUpdateOwnImage(const QJsonObject &json);//修改个人头像

    void receiveUpdateGroupName(const QJsonObject &json);//修改群昵称
    void receiveUpdateGroupImage(const QJsonObject &json);//修改群头像

    void receiveFriend(const QJsonObject &json);//接收好友信息
    void receiveAddFriend(const QJsonObject &json);//接收添加好友信息
    void receiveDelFriend(const QJsonObject &json);//接收删除好友信息

    void receiveCreateGroup(const QJsonObject &json);//接收创建群信息
    void receiveGroup(const QJsonObject &json);//接收群信息
    void receiveAddGroup(const QJsonObject &json);//接收添加群信息
    void receiveDelGroup(const QJsonObject &json);//接收删除群信息

    void receiveFriends(const QJsonObject &json);//接收好友列表信息
    void receiveGroups(const QJsonObject &json);//接收群列表信息
    void receiveFriendsOfGroup(const QJsonObject &json);//接收群内好友列表信息

    void receiveChatFriend(const QJsonObject &json);//接收好友聊天信息
    void receiveChatGroup(const QJsonObject &json);//接收群内聊天信息

    void pixmapToByteArray(QByteArray &imageByteArray,
                           const QPixmap &pixmap, const QString &imageType);//QPixmap 转 QByteArray，传图片

    int stringToInt(const QString &byteArray);//QByteArray 转 uint,用于类型和长度判断
    qulonglong stringToQulonglong(const QString &string);//QString 转 qulonglong,用于Id判断

signals:
    void toLoginId(qulonglong id);//传送注册账号
    void toUiUpdate(int type, QString information, qulonglong groupId = 0);//传给Ui界面数据信号
    void toHomepageFriend(int type, qulonglong friendId, const QString &friendName,
                          const QString &friendSign, const QPixmap &friendImage, qulonglong groupId = 0);//传给 friend
    void toHomepageGroup(int type, qulonglong groupId,
                   const QString &groupName, const QPixmap &groupImage);//传给 group
    void toHomepageChat(int type, qulonglong sendId,
                  QString sendName, const QString &msg,
                  qulonglong groupId = 0, QString groupName = "");//传送聊天信息

private:
    qulonglong ownId;//用户账号
};

#endif // SOCKET_H
