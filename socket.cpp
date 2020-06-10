#include "socket.h"
//#define Q_QDOC

#include <QJsonObject>
#include <QJsonParseError>
#include <QHostAddress>
#include <QVariant>
#include <QBuffer>
#include <QPixmap>

const int signUpType = 9000000;//注册
const int loginType = 9000001;//登录
const int modifyOwnPasswordType = 9000002;//修改个人密码
const int modifyOwnNameType = 9000003;//修改个人昵称
const int modifyOwnSignType = 9000004;//修改个人签名
const int modifyGroupNameType = 9000005;//修改群昵称
const int searchFriendType = 9000006;//查找好友
const int addFriendType = 9000007;//添加好友
const int delFriendType = 9000008;//删除好友
const int createGroupType = 9000009;//创建群聊
const int searchGroupType = 9000010;//查找群
const int addGroupType = 9000011;//添加群
const int delGroupType = 9000012;//删除群
const int searchFriendsType = 9000013;//查找好友列表
const int searchGroupsType = 9000014;//查找群列表
const int searchFriendsOfGroupType = 9000015;//查找群内好友列表
const int chatFriendType = 9000016;//好友聊天
const int chatGroupType = 9000017;//群内聊天
const int modifyOwnImageType = 9000018;//修改个人头像
const int modifyGroupImageType = 9000019;//修改群头像

const int unknownType = 9999998;//未知类型,数据错误

Socket::Socket(QTcpSocket *parent) :
    QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(receiveData()));
    setReadBufferSize(999999);
}

Socket::~Socket()
{

}

void Socket::signUp(const QString &ownName, const QString &ownPassword)
{
    QJsonObject json;
    json.insert("type",QString::number(signUpType));
    json.insert("ownName",ownName);
    json.insert("ownPassword",ownPassword);
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::login(qulonglong ownId, const QString &ownPassword)
{
    QJsonObject json;
    json.insert("type",QString::number(loginType));
    json.insert("ownId",QString::number(ownId));
    json.insert("ownPassword",ownPassword);
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::updateOwnPassword(qulonglong ownId, const QString &oldPassword, const QString &newPassword)
{
    QJsonObject json;
    json.insert("type",QString::number(modifyOwnPasswordType));
    json.insert("ownId",QString::number(ownId));
    json.insert("oldPassword",oldPassword);
    json.insert("newPassword",newPassword);
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::updateOwnName(qulonglong ownId, const QString &newName)
{
    QJsonObject json;
    json.insert("type",QString::number(modifyOwnNameType));
    json.insert("ownId",QString::number(ownId));
    json.insert("newName",newName);
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::updateOwnSign(qulonglong ownId, const QString &newSign)
{
    QJsonObject json;
    json.insert("type",QString::number(modifyOwnSignType));
    json.insert("ownId",QString::number(ownId));
    json.insert("newSign",newSign);
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::updateOwnImage(qulonglong ownId, const QPixmap &newImage, const QString &imageType)
{
    QByteArray imageByteArray;
    pixmapToByteArray(imageByteArray,newImage,imageType);

    QJsonObject json;
    json.insert("type",QString::number(modifyOwnImageType));
    json.insert("ownId",QString::number(ownId));
    json.insert("newImage",QString::fromLatin1(imageByteArray,imageByteArray.size()));
    json.insert("imageType",imageType);
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::updateGroupName(qulonglong groupId, const QString &newName)
{
    QJsonObject json;
    json.insert("type",QString::number(modifyGroupNameType));
    json.insert("groupId",QString::number(groupId));
    json.insert("newName",newName);
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::updateGroupImage(qulonglong groupId, const QPixmap &newImage, const QString &imageType)
{
    QByteArray imageByteArray;
    pixmapToByteArray(imageByteArray,newImage,imageType);

    QJsonObject json;
    json.insert("type",QString::number(modifyGroupImageType));
    json.insert("groupId",QString::number(groupId));
    json.insert("newImage",QString::fromLatin1(imageByteArray,imageByteArray.size()));
    json.insert("imageType",imageType);
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::searchFriend(qulonglong friendId)
{
    QJsonObject json;
    json.insert("type",QString::number(searchFriendType));
    json.insert("friendId",QString::number(friendId));
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::addFriend(qulonglong requestId, qulonglong friendId)
{
    QJsonObject json;
    json.insert("type",QString::number(addFriendType));
    json.insert("requestId",QString::number(requestId));
    json.insert("friendId",QString::number(friendId));
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::delFriend(qulonglong friendId)
{
    QJsonObject json;
    json.insert("type",QString::number(delFriendType));
    json.insert("friendId",QString::number(friendId));
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::createGroup(const QString &groupName)
{
    QJsonObject json;
    json.insert("type",QString::number(createGroupType));
    json.insert("groupName",groupName);
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::searchGroup(qulonglong groupId)
{
    QJsonObject json;
    json.insert("type",QString::number(searchGroupType));
    json.insert("groupId",QString::number(groupId));
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::addGroup(qulonglong requestId, qulonglong groupId)
{
    QJsonObject json;
    json.insert("type",QString::number(addGroupType));
    json.insert("requestId",QString::number(requestId));
    json.insert("groupId",QString::number(groupId));
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::delGroup(qulonglong groupId)
{
    QJsonObject json;
    json.insert("type",QString::number(delGroupType));
    json.insert("groupId",QString::number(groupId));
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::getFriends()
{
    QJsonObject json;
    json.insert("type",QString::number(searchFriendsType));
    json.insert("ownId",QString::number(ownId));
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::getGroups()
{
    QJsonObject json;
    json.insert("type",QString::number(searchGroupsType));
    json.insert("ownId",QString::number(ownId));
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::getFriendsOfGroup(qulonglong groupId)
{
    QJsonObject json;
    json.insert("type",QString::number(searchFriendsOfGroupType));
    json.insert("groupId",QString::number(groupId));
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::chatFriend(qulonglong friendId, const QString &friendName, const QString &msg)
{
    qDebug()<<"发送消息2";
    QJsonObject json;
    json.insert("type",QString::number(chatFriendType));
    json.insert("friendId",QString::number(friendId));
    json.insert("friendName",friendName);
    json.insert("msg",msg);
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::chatGroup(qulonglong groupId, const QString &groupName, const QString &msg)
{
    QJsonObject json;
    json.insert("type",QString::number(chatGroupType));
    json.insert("groupId",QString::number(groupId));
    json.insert("groupName",groupName);
    json.insert("msg",msg);
    QByteArray data = QJsonDocument(json).toJson();
    sendData(data);
}

void Socket::sendData(const QByteArray &data)
{
    QString len = QString::number(data.size());
    len = QString(7-len.size(),'0')+len;
    write(len.toLocal8Bit(),7);
    if(data.size() > 0) write(data,data.size());
}

QByteArray readDataLength(QByteArray &data, uint dataLength)
{
    QByteArray rec;
    if(data.size() <= (int)dataLength){
        rec = data;
        data.clear();
    }
    else{
        for(uint i = 0;i < dataLength;++i) rec.push_back(data[i]);
        for(uint i = dataLength;i < (uint)data.size();++i) data[i-dataLength] = data[i];
        data.resize((uint)data.size()-dataLength);
    }
    return rec;
}

QByteArray lastRemain = "";
ushort where = 0;
uint length = 7;

void Socket::receiveData()
{
    QByteArray byteAll = readAll();
    byteAll = lastRemain+byteAll;

    while(byteAll.size() > 0){
        QByteArray data;
        switch(where){
        case 0:{
            data = readDataLength(byteAll,7);
            if(data.size() < 7){
                lastRemain = data;
                where = 0;
                return ;
            }
            lastRemain.clear();
            where = 0;
            length = stringToInt(data);
            if(length == 0) {
                where = 0;
                continue ;
            }
        }
        case 1:{
            data = readDataLength(byteAll,length);
            if(data.size() < (int)length){
                lastRemain = data;
                where = 1;
                return ;
            }
            lastRemain.clear();
            where = 0;

            QJsonObject json = QJsonDocument::fromJson(data).object();
            int type = stringToInt(json.value("type").toString());
            if(type == 0) continue;

            //qDebug()<<json;

            switch(type){
            case signUpType: receiveSignUp(json);break;//注册账号
            case loginType: receiveLogin(json);break;//登录请求
            case modifyOwnPasswordType: receiveUpdateOwnPassword(json);break;//修改个人密码
            case modifyOwnNameType: receiveUpdateOwnName(json);break;//修改个人昵称
            case modifyOwnSignType: receiveUpdateOwnSign(json);break;//修改个人签名
            case modifyGroupNameType: receiveUpdateGroupName(json);break;//修改群昵称
            case searchFriendType: receiveFriend(json);break;//查找好友
            case addFriendType: receiveAddFriend(json);break;//添加好友
            case delFriendType: receiveDelFriend(json);break;//删除好友
            case createGroupType: receiveCreateGroup(json);break;//创建群聊
            case searchGroupType: receiveGroup(json);break;//查找群
            case addGroupType: receiveAddGroup(json);break;//添加群
            case delGroupType: receiveDelGroup(json);break;//删除群
            case searchFriendsType: receiveFriends(json);break;//查找好友列表
            case searchGroupsType: receiveGroups(json);break;//查找群列表
            case searchFriendsOfGroupType: receiveFriendsOfGroup(json);break;//查找群内好友列表
            case chatFriendType: receiveChatFriend(json);break;//好友聊天
            case chatGroupType: receiveChatGroup(json);break;//群内聊天
            case modifyOwnImageType: receiveUpdateOwnImage(json);break;//修改个人头像
            case modifyGroupImageType: receiveUpdateGroupImage(json);break;//修改群头像
            case unknownType: emit toUiUpdate(unknownType,"");break;
            default:break;
            }
        }
        }
    }
}

void Socket::receiveSignUp(const QJsonObject &json)
{
    if(json.contains("information")){
        emit toUiUpdate(signUpType,json.value("information").toString());
        return ;
    }
    qulonglong id = json.value("ownId").toString().toULongLong();
    emit toLoginId(id);
}

void Socket::receiveLogin(const QJsonObject &json)
{
    QString information = json.value("information").toString();
    emit toUiUpdate(loginType,information);
}

void Socket::receiveUpdateOwnPassword(const QJsonObject &json)
{
    QString information = json.value("information").toString();
    emit toUiUpdate(modifyOwnPasswordType,information);
}

void Socket::receiveUpdateOwnName(const QJsonObject &json)
{
    QString information = json.value("information").toString();
    emit toUiUpdate(modifyOwnNameType,information);
}

void Socket::receiveUpdateOwnSign(const QJsonObject &json)
{
    QString information = json.value("information").toString();
    emit toUiUpdate(modifyOwnSignType,information);
}

void Socket::receiveUpdateOwnImage(const QJsonObject &json)
{
    QString information = json.value("information").toString();
    emit toUiUpdate(modifyOwnImageType,information);
}

void Socket::receiveUpdateGroupName(const QJsonObject &json)
{
    if(json.contains("information")){
        emit toUiUpdate(modifyGroupNameType,json.value("information").toString());
        return ;
    }
    qulonglong groupId= json.value("groupId").toString().toULongLong();
    QString groupName = json.value("groupName").toString();
    emit toUiUpdate(modifyGroupNameType,groupName,groupId);
}

void Socket::receiveUpdateGroupImage(const QJsonObject &json)
{
    if(json.contains("information")){
        emit toUiUpdate(modifyGroupImageType,json.value("information").toString());
        return ;
    }
    qulonglong groupId= json.value("groupId").toString().toULongLong();
    QString groupImage = json.value("groupImage").toString();
    emit toUiUpdate(modifyGroupImageType,groupImage,groupId);
}

void Socket::receiveFriend(const QJsonObject &json)
{
    if(json.contains("information")){
        emit toUiUpdate(searchFriendType,json.value("information").toString());
        return ;
    }
    qulonglong friendId = json.value("friendId").toString().toULongLong();
    QString friendName = json.value("friendName").toString();
    QString friendSign = json.value("friendSign").toString();
    QPixmap friendImage;
    friendImage.loadFromData(json.value("friendImage").toString().toLatin1());
    emit toHomepageFriend(searchFriendType,friendId,friendName,friendSign,friendImage);
}

void Socket::receiveAddFriend(const QJsonObject &json)
{
    if(json.contains("information")){
        emit toUiUpdate(addFriendType,json.value("information").toString());
        return ;
    }
    qulonglong friendId = json.value("friendId").toString().toULongLong();
    QString friendName = json.value("friendName").toString();
    QString friendSign = json.value("friendSign").toString();
    QPixmap friendImage;
    friendImage.loadFromData(json.value("friendImage").toString().toLatin1());
    emit toHomepageFriend(addFriendType,friendId,friendName,friendSign,friendImage);
}

void Socket::receiveDelFriend(const QJsonObject &json)
{
    if(json.contains("information")){
        emit toUiUpdate(delFriendType,json.value("information").toString());
        return ;
    }
    qulonglong friendId = json.value("friendId").toString().toULongLong();
    emit toHomepageFriend(delFriendType,friendId,"","",QPixmap());
}

void Socket::receiveCreateGroup(const QJsonObject &json)
{
    if(json.contains("information")){
        emit toUiUpdate(createGroupType,json.value("information").toString());
        return ;
    }
    qulonglong groupId = json.value("groupId").toString().toULongLong();
    QString groupName = json.value("groupName").toString();
    emit toHomepageGroup(createGroupType,groupId,groupName,QPixmap());
}

void Socket::receiveGroup(const QJsonObject &json)
{
    if(json.contains("information")){
        emit toUiUpdate(searchGroupType,json.value("information").toString());
        return ;
    }
    qulonglong groupId = json.value("groupId").toString().toULongLong();
    QString groupName = json.value("groupName").toString();
    QPixmap groupImage;
    groupImage.loadFromData(json.value("groupImage").toString().toLatin1());
    emit toHomepageGroup(searchGroupType,groupId,groupName,groupImage);
}

void Socket::receiveAddGroup(const QJsonObject &json)
{
    if(json.contains("information")){
        emit toUiUpdate(addGroupType,json.value("information").toString());
        return ;
    }
    qulonglong groupId = json.value("groupId").toString().toULongLong();
    QString groupName = json.value("groupName").toString();
    QPixmap groupImage;
    groupImage.loadFromData(json.value("groupImage").toString().toLatin1());
    emit toHomepageGroup(addGroupType,groupId,groupName,groupImage);
}

void Socket::receiveDelGroup(const QJsonObject &json)
{
    if(json.contains("information")){
        emit toUiUpdate(delGroupType,json.value("information").toString());
        return ;
    }
    qulonglong groupId = json.value("groupId").toString().toULongLong();
    emit toHomepageGroup(delGroupType,groupId,"",QPixmap());
}

void Socket::receiveFriends(const QJsonObject &json)
{
    if(json.contains("information")){
        emit toUiUpdate(searchFriendsType,json.value("information").toString());
        return ;
    }
    qulonglong friendId = json.value("friendId").toString().toULongLong();
    QString friendName = json.value("friendName").toString();
    QString friendSign = json.value("friendSign").toString();
    QPixmap friendImage;
    friendImage.loadFromData(json.value("friendImage").toString().toLatin1());
    emit toHomepageFriend(searchFriendsType,friendId,friendName,friendSign,friendImage);
}

void Socket::receiveGroups(const QJsonObject &json)
{
    if(json.contains("information")){
        emit toUiUpdate(searchGroupsType,json.value("information").toString());
        return ;
    }
    qulonglong groupId = json.value("groupId").toString().toULongLong();
    QString groupName = json.value("groupName").toString();
    QPixmap groupImage;
    groupImage.loadFromData(json.value("groupImage").toString().toLatin1());
    emit toHomepageGroup(searchGroupsType,groupId,groupName,groupImage);
}

void Socket::receiveFriendsOfGroup(const QJsonObject &json)
{
    if(json.contains("information")){
        emit toUiUpdate(searchFriendsOfGroupType,json.value("information").toString());
        return ;
    }
    qulonglong friendId = json.value("friendId").toString().toULongLong();
    QString friendName = json.value("friendName").toString();
    QString friendSign = json.value("friendSign").toString();
    QPixmap friendImage;
    friendImage.loadFromData(json.value("friendImage").toString().toLatin1());
    qulonglong groupId = json.value("groupId").toString().toULongLong();
    emit toHomepageFriend(searchFriendsOfGroupType,friendId,friendName,friendSign,friendImage,groupId);
}

void Socket::receiveChatFriend(const QJsonObject &json)
{
    if(json.contains("information")){
        emit toUiUpdate(chatFriendType,json.value("information").toString());
        return ;
    }
    qulonglong sendId = json.value("sendId").toString().toULongLong();
    QString sendName = json.value("sendName").toString();
    QString msg = json.value("msg").toString();
    emit toHomepageChat(chatFriendType,sendId,sendName,msg);
}

void Socket::receiveChatGroup(const QJsonObject &json)
{
    if(json.contains("information")){
        emit toUiUpdate(chatGroupType,json.value("information").toString());
        return ;
    }
    qulonglong sendId = json.value("sendId").toString().toULongLong();
    QString sendName = json.value("sendName").toString();
    QString msg = json.value("msg").toString();
    qulonglong groupId = json.value("groupId").toString().toULongLong();
    QString groupName = json.value("groupName").toString();
    emit toHomepageChat(chatGroupType,sendId,sendName,msg,groupId,groupName);
}

void Socket::pixmapToByteArray(QByteArray &imageByteArray, const QPixmap &pixmap, const QString &imageType)
{
    QBuffer buffer;
    pixmap.save(&buffer,imageType.toStdString().c_str());
    imageByteArray.resize(buffer.data().size());
    imageByteArray = buffer.data();
}

int Socket::stringToInt(const QString &string)
{
    bool isSuccess;
    int number = string.toUInt(&isSuccess,10);//数据长度
    qDebug()<<string;
    qDebug()<<QString::number(number,10);
    if(!isSuccess) {
        qDebug()<<"QString转换int失败";
        return 0;
    }
    else {
        qDebug()<<"QString转换int成功";
        return number;
    }
}

qulonglong Socket::stringToQulonglong(const QString &string)
{
    bool isSuccess;
    qulonglong number = string.toULongLong(&isSuccess,10);
    qDebug()<<string;
    qDebug()<<QString::number(number,10);
    if(!isSuccess) {
        qDebug()<<"QString转换qulonglong失败";
        return 0;
    }
    else {
        qDebug()<<"QString转换qulonglong成功";
        return number;
    }
}


