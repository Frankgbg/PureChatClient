#include "homepage.h"
#include "mywidget.h"
#include "ui_homepage.h"

#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>
#include <QBitmap>

const QByteArray successful = "Login successful.";
const QByteArray failed = "Login failed!";
const QByteArray notExist = "Account/Group does not exists!";
const QByteArray logged = "Account already logged in!";
const QByteArray databaseFailed = "Database operation failed!";
const QByteArray friendExist = "Friend exists!";
const QByteArray groupExist = "Group exists!";
const QByteArray exist = "Account/Group exists.";

const QByteArray unknownError = "Unknown error!";
const QByteArray numberError = "Account/Group number error!";
const QByteArray requestSuccessful = "Request successful!";
const QByteArray requestFailed = "Request failed!";
const QByteArray signUpFailed = "Sign up failed!";
const QByteArray createFailed = "Create group failed!";
const QByteArray complete = "Data transmission completed";

//const int signUpType = 9000000;//注册
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

Homepage::Homepage(qulonglong id, QString password, Socket *sock, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Homepage)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);//无窗口边框,无任务栏图标

    desktopHeight = 0;
    ownId = id;
    ownPassword = password;

    /*左上角图标*/
    QPixmap pixmap(":/homepage/P_24px_blue.ico");
    ui->label->setPixmap(pixmap);

    ui->lineEdit_2->setReadOnly(true);

    /*注册事件过滤器*/
    ui->label_2->installEventFilter(this);//最小化
    ui->label_3->installEventFilter(this);//关闭
    ui->label_7->installEventFilter(this);//好友
    ui->label_8->installEventFilter(this);//群组
    ui->label_9->installEventFilter(this);//待开发

    /*隐藏动画*/
    animation = new QPropertyAnimation(this, "geometry",this);

    /*任务栏图标*/
    QSystemTrayIcon *sysTrayIcon = new QSystemTrayIcon(this);
    sysTrayIcon->setIcon(QIcon(":/homepage/chat_72px.ico"));
    sysTrayIcon->show();

    QMenu *menuTray = new QMenu;//图标菜单
    QAction *actQuit = new QAction("退出", menuTray);
    QAction *actOffline = new QAction("离线", menuTray);
    QAction *actOnline = new QAction("上线", menuTray);
    menuTray->addAction(actOnline);//菜单中动作
    menuTray->addAction(actOffline);//菜单中动作
    menuTray->addAction(actQuit);//菜单中动作
    sysTrayIcon->setContextMenu(menuTray);
    sysTrayIcon->setToolTip("PureChat");

    connect(sysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(onSysTrayIconActived(QSystemTrayIcon::ActivationReason)));
    connect(actQuit, SIGNAL(triggered()), this, SLOT(quit()));
    connect(actOffline, SIGNAL(triggered()), this, SLOT(offline()));
    connect(actOnline, SIGNAL(triggered()), this, SLOT(online()));
    
    /*快速搜索好友，群组*/
    //connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(showResearch()));

    /*三页列表*/
    qstacklayout = new QStackedLayout;
    ui->widget->setLayout(qstacklayout);
    friends = new FriendList(350,63,true);//好友管理
    groups= new GroupList(350,56,false);//群组管理
    connect(friends,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(showChatFriend(QListWidgetItem*)));
    connect(friends,SIGNAL(showInformation(MyWidget*)),this,SLOT(showInformation(MyWidget*)));
    connect(friends,SIGNAL(delFriend(qulonglong)),this,SLOT(delFriend(qulonglong)));
    connect(groups,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(showChatGroup(QListWidgetItem*)));
    connect(groups,SIGNAL(showInformation(MyWidget*)),this,SLOT(showInformation(MyWidget*)));
    connect(groups,SIGNAL(delGroup(qulonglong)),this,SLOT(delGroup(qulonglong)));
    qstacklayout->addWidget(friends);//第一页好友
    qstacklayout->addWidget(groups);//第二页群组

    socket = sock;
    connectSocketSignals(socket);

    /*保持连接以及重连*/
    socketThread = new SocketThread(socket,this);
    socketThread->start();

    chatFriend = new QHash<qulonglong, ChatFriend*>;
    chatGroup = new QHash<qulonglong, ChatGroup*>;
    friendInformation = new QHash<qulonglong, Information*>;
    groupInformation = new QHash<qulonglong, Information*>;

    search = new Search();
    connect(search,SIGNAL(updateSearch(int,qulonglong)),this,SLOT(updateSearch(int,qulonglong)));
    search->hide();

    create = new CreateGroup;
    connect(create,SIGNAL(createGroup(QString)),this,SLOT(createGroup(QString)));
    create->hide();

    move(QPoint(1300,100));
}

Homepage::~Homepage()
{
    delete ui;

    delete qstacklayout;
    qstacklayout = nullptr;

    delete friends;
    friends = nullptr;

    delete groups;
    groups = nullptr;

    while(!chatFriend->empty()){
        delete chatFriend->begin().value();
        chatFriend->erase(chatFriend->begin());
    }
    delete chatFriend;
    chatFriend = nullptr;

    while(chatGroup->count() > 0){
        delete chatGroup->begin().value();
        chatGroup->erase(chatGroup->begin());
    }
    delete chatGroup;
    chatGroup = nullptr;

    while(friendInformation->count() > 0){
        delete friendInformation->begin().value();
        friendInformation->erase(friendInformation->begin());
    }
    delete friendInformation;
    friendInformation = nullptr;

    while(groupInformation->count() > 0){
        delete groupInformation->begin().value();
        groupInformation->erase(groupInformation->begin());
    }
    delete groupInformation;
    groupInformation = nullptr;

    delete search;
    search = nullptr;

    delete create;
    create = nullptr;

    if(ownInformation != nullptr) {
        delete ownInformation;
        ownInformation = nullptr;
    }

    delete socketThread;
    socketThread = nullptr;
}

void Homepage::mousePressEvent(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton){
        offSet = event->globalPos()-this->pos();
    }
}

void Homepage::mouseReleaseEvent(QMouseEvent *event)
{
    if(y() < 0)
    {
        qDebug()<<"y的坐标等于0";
        desktopHeight = -height()+1;
        animation->setDuration(400);//动画时间
        animation->setStartValue(QRect(x(), 0, width(), height()));
        animation->setEndValue(QRect(x(), desktopHeight, width(), height()));
        move(x(),0);
        animation->start();
    }
    else if(y() > 0){
        desktopHeight = y();
    }
    event->ignore();
}

void Homepage::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        this->move(event->globalPos()-offSet);//移动主窗体位置
    }
}

void Homepage::enterEvent(QEvent *event)
{
    if(desktopHeight < 0 && y() < 0){
        animation->setDirection(QPropertyAnimation::Backward);//反向出现
        animation->start();
    }
    event->ignore();
}

void Homepage::leaveEvent(QEvent *event)
{
    if(desktopHeight < 0 && y() >= 0){
        animation->setDirection(QPropertyAnimation::Forward);//正向隐藏
        animation->start();
    }
    event->ignore();
}

void Homepage::focusOutEvent(QFocusEvent *event)
{
    if(desktopHeight < 0 && y() >= 0){
        animation->setDirection(QPropertyAnimation::Forward);//正向隐藏
        animation->start();
    }
    event->ignore();
}

bool Homepage::eventFilter(QObject *object, QEvent *e)
{
    if(object == ui->label_2){//最小化
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            return true;
        }
    }
    else if(object == ui->label_3){//关闭
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){
            QMessageBox messageBox(QMessageBox::NoIcon,"确认信息!","是否退出程序?",QMessageBox::Yes|QMessageBox::No);
            messageBox.move(QPoint(x()+75,y()+250));
            QApplication::setQuitOnLastWindowClosed(false);
            int result = messageBox.exec();
            QApplication::setQuitOnLastWindowClosed(true);
            switch(result){
            case QMessageBox::Yes:{
                quit();
                break;
            }
            case QMessageBox::No:break;
            default: break;
            }
            return true;
        }
    }
    else if(object == ui->label_7){//第一页好友列表
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            qstacklayout->setCurrentIndex(0);
            return true;
        }
    }
    else if(object == ui->label_8){//第二页群列表
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){
            qstacklayout->setCurrentIndex(1);
            return true;
        }
    }
    else if(object == ui->label_9){//第三页群列表
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){
            /*
             * qstacklayout->setCurrentIndex(2);
             * 暂未开发
             */
            return true;
        }
    }
    else if(e->type()==QEvent::WindowDeactivate){
        qDebug()<<"失去焦点";
        if(desktopHeight < 0 && y() >= 0){
            animation->setDirection(QPropertyAnimation::Forward);//正向隐藏
            animation->start();
        }
    }
    return false;
}

void Homepage::paintEvent(QPaintEvent *event)
{
    QPainterPath path;//路径
    path.setFillRule(Qt::WindingFill);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::transparent));

    QColor color(0, 0, 0, 75);

    int x = 1;
    for(int i = 0; i < x; i++)
    {
        QPainterPath path;
        path.addRect(i,i,this->width()-i*2,this->height()-i*2);//外面一圈
        color.setAlpha(255);//透明度,100%
        painter.setPen(color);
        painter.drawPath(path);
    }
    event->ignore();
}


void Homepage::onSysTrayIconActived(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
        case QSystemTrayIcon::Trigger://单击
        case QSystemTrayIcon::DoubleClick:{//双击
            if(desktopHeight < 0 && y() < 0){
                animation->setDirection(QPropertyAnimation::Backward);//反向出现
                animation->start();
                activateWindow();
                raise();
            }
            else toping(this);
            break;
        }
        default:break;
    }
}

void Homepage::delFriend(qulonglong friendId)
{
    socket->delFriend(friendId);
}

void Homepage::delGroup(qulonglong groupId)
{
    socket->delGroup(groupId);
}

void Homepage::showChatFriend(QListWidgetItem *_friend)
{
    MyWidget *myWidget = static_cast<MyWidget*>(friends->itemWidget(_friend));
    showChatFriend(myWidget);
}

void Homepage::showChatFriend(MyWidget *myWidget)
{
    ChatFriend *chat = existChatFriend(myWidget->getId());
    if(chat != nullptr){
        toping(chat);
        return ;
    }
    chat = newChatFriend(myWidget->getId(),myWidget->getPixmap(),myWidget->getName());
    chat->show();
}

void Homepage::showChatGroup(QListWidgetItem *_group)
{
    MyWidget *myWidget = static_cast<MyWidget*>(groups->itemWidget(_group));
    ChatGroup *chat = existChatGroup(myWidget->getId());
    if(chat != nullptr){
        toping(chat);
        return ;
    }
    chat = newChatGroup(myWidget->getId(),myWidget->getPixmap(),myWidget->getName());
    socket->getFriendsOfGroup(myWidget->getId());
    chat->show();
}

void Homepage::showInformation(MyWidget *currentWidget)
{
    if(qstacklayout->currentIndex() == 0) showInformation(false,currentWidget);
    else showInformation(true,currentWidget);
}

void Homepage::showInformation(bool editabled, MyWidget *currentWidget)
{
    Information *information = nullptr;
    if(!editabled) information = existFriendInformation(currentWidget->getId());
    else information = existGroupInformation(currentWidget->getId());
    if(information != nullptr){
        toping(information);
        return ;
    }
    information = newInformation(currentWidget->getId(),currentWidget->getPixmap(),
                                                 currentWidget->getName(),currentWidget->getSign(),editabled);
    information->show();
}

void Homepage::removeChatFriend(qulonglong friendId)
{
    chatFriend->remove(friendId);
}

void Homepage::removeChatGroup(qulonglong groupId)
{
    chatGroup->remove(groupId);
}

void Homepage::removeInformation(bool editabled, qulonglong id)
{
    if(editabled) groupInformation->remove(id);
    else friendInformation->remove(id);
}

void Homepage::sendMsg(int type, qulonglong id, const QString &name, const QString &msg)
{
    qDebug()<<"发送了消息";
    switch(type){
    case chatFriendType:{
        socket->chatFriend(id,name,msg);
        break;
    }
    case chatGroupType:{
        socket->chatGroup(id,name,msg);
        break;
    }
    }
}

void Homepage::updateInformation(int type, qulonglong id, const QString &information)
{
    switch(type){
    case modifyOwnPasswordType:{
        socket->updateOwnPassword(id,ownPassword,information);
        break;
    }
    case modifyOwnNameType:{
        socket->updateOwnName(id,information);
        break;
    }
    case modifyOwnSignType:{
        socket->updateOwnSign(id,information);
        break;
    }
    case modifyGroupNameType:{
        socket->updateGroupName(id,information);
        break;
    }
    }
}

void Homepage::updateInformationSuccessful(int type, qulonglong id, const QString &information)
{
    switch(type){
    case modifyOwnNameType:{
        ownName = information;
        ui->label_5->setText(ownName);
        friends->updateName(ownId,ownName);
        break;
    }
    case modifyOwnSignType:{
        ownSign = information;
        ui->lineEdit_2->setText(ownSign);
        friends->updateSign(ownId,ownSign);
        break;
    }
    case modifyOwnPasswordType:{
        ownPassword = information;
        break;
    }
    case modifyGroupNameType:{
        groups->updateName(id,information);
        break;
    }
    }
}

void Homepage::updateImage(int type, qulonglong id, const QPixmap &newImage, const QString &imageType)
{
    switch(type){
    case modifyOwnImageType:{
        socket->updateOwnImage(id,newImage,imageType);
        break;
    }
    case modifyGroupImageType:{
        socket->updateGroupImage(id,newImage,imageType);
        break;
    }
    }
}

void Homepage::updateImageSuccessful(int type, qulonglong id, const QPixmap &newImage)
{
    switch(type){
    case modifyOwnImageType:{
        ownImage = newImage;
        QPixmap result;
        imageProcessing(result,ownImage);
        ui->label_4->setPixmap(result);
        friends->updatePixmap(ownId,ownImage);
        break;
    }
    case modifyGroupImageType:{
        groups->updatePixmap(id,newImage);
        break;
    }
    }
}

void Homepage::updateSearch(int type, qulonglong id)
{
    switch(type){
    case searchFriendType:{
        socket->searchFriend(id);
        break;
    }
    case addFriendType:{
        socket->addFriend(ownId,id);
        break;
    }
    case searchGroupType:{
        socket->searchGroup(id);
        break;
    }
    case addGroupType:{
        socket->addGroup(ownId,id);
        break;
    }
    }
}

void Homepage::createGroup(QString groupName)
{
    socket->createGroup(groupName);
}

void Homepage::connection()
{
    socket->login(ownId,ownPassword);
    /*左上角图标*/
    QPixmap pixmap(":/homepage/P_24px_blue.ico");
    ui->label->setPixmap(pixmap);
}

void Homepage::disconnection()
{
    /*左上角图标*/
    QPixmap pixmap(":/homepage/P_24px_white.ico");
    ui->label->setPixmap(pixmap);
}

void Homepage::socketUpdate(int type, QString information, qulonglong groupId)
{
    switch(type){
    case loginType:{
        if(information == successful) {
            tips("重新登录成功.",QPoint(x()+75,y()+250));
        }
        else if(information == failed) {
            tips("重连失败!",QPoint(x()+75,y()+250));
            socket->close();
        }
        else if(information == logged) {
            tips("账号已登录!",QPoint(x()+75,y()+250));
            socket->close();
        }
        else {
            tips("未知错误,断开连接!",QPoint(x()+75,y()+250));
            socket->close();
        }
        break;
    }
    case modifyOwnPasswordType:
    case modifyOwnNameType:
    case modifyOwnSignType:
    case modifyOwnImageType:{
         ownInformation->receive(type,information);
         break;
    }
    case modifyGroupNameType:
    case modifyGroupImageType:{
        Information *groupInformation = existGroupInformation(groupId);
        if(groupId != 0){
            if(groupInformation != nullptr) {
                groupInformation->receive(type,requestSuccessful);
                return ;
            }
            if(type == modifyGroupNameType){
                groups->updateName(groupId,information);
            }
            else if(type == modifyGroupImageType){
                QPixmap image;
                image.loadFromData(information.toLatin1());
                groups->updatePixmap(groupId,image);
            }
        }
        else{
            if(groupInformation != nullptr) groupInformation->receive(type,requestFailed);
        }
        break;
    }
    case searchFriendType:{
        if(information == notExist){
            QPixmap image;
            search->receiveFriend(0,image,"","");//账号不存在
        }
        else{
            QPixmap image;
            search->receiveFriend(1,image,"","");//提示重新查找
        }
        break;
    }
    case addFriendType:{
        if(information == friendExist){
            tips("已是好友!",QCursor::pos()+QPoint(-180,-120));
        }
        else{
            tips("重新添加.",QCursor::pos()+QPoint(-180,-120));
        }
        break;
    }
    case delFriendType:{
        if(information == notExist){
            tips("好友不存在!",QCursor::pos()+QPoint(-180,-120));
        }
        else{
            tips("重新删除.",QCursor::pos()+QPoint(-180,-120));
        }
        break;
    }
    case createGroupType:{
        if(information == requestFailed){
            tips("创建群失败!",QCursor::pos()+QPoint(-180,-120));
        }
        break;
    }
    case searchGroupType:{
        if(information == notExist){
            QPixmap image;
            search->receiveGroup(0,image,"");//账号不存在
        }
        else{
            QPixmap image;
            search->receiveGroup(1,image,"");//提示重新查找
        }
        break;
    }
    case addGroupType:{
        if(information == groupExist){
            tips("已在群中!",QCursor::pos()+QPoint(-180,-120));
        }
        else{
            tips("重新添加.",QCursor::pos()+QPoint(-180,-120));
        }
        break;
    }
    case delGroupType:{
        tips("重新删除.",QPoint(x()+75,y()+250));
        break;
    }
    case searchFriendsType:{
        tips("好友列表获取失败!",QPoint(x()+75,y()+250));
        break;
    }
    case searchGroupsType:{
        tips("群列表获取失败!",QPoint(x()+75,y()+250));
        break;
    }
    case searchFriendsOfGroupType:{
        tips("群内好友列表获取失败!",QCursor::pos()+QPoint(-180,-120));
        break;
    }
    case unknownType:{
        tips("未知错误");
        break;
    }
    }
}

void Homepage::socketFriend(int type, qulonglong friendId, const QString &friendName,
                            const QString &friendSign, const QPixmap &friendImage, qulonglong groupId)
{
    if(type == searchFriendType){
        if(friendImage.isNull()) {
            QPixmap image(":/headportrait/head.jpg");
            search->receiveFriend(friendId,image,friendName,friendSign);
        }
        else search->receiveFriend(friendId,friendImage,friendName,friendSign);
    }
    else if(type == searchFriendsType || type == addFriendType){
        if(friendImage.isNull()) friends->addWidget(friendId,QPixmap(":/headportrait/head.jpg"),friendName,friendSign);
        else friends->addWidget(friendId,friendImage,friendName,friendSign);
        if(friendId == ownId){
            ownName = friendName;
            ownSign = friendSign;
            ownImage = friendImage;
            QPixmap result;
            if(friendImage.isNull()) imageProcessing(result,QPixmap(":/headportrait/head.jpg"));
            else imageProcessing(result,ownImage);
            ui->label_4->setPixmap(result);
            ui->label_5->setText(ownName);
            ui->lineEdit_2->setText(ownSign);

            ownInformation = new OwnInformation(ownId,ownImage,ownName,ownSign,ownPassword);

            connect(ownInformation,SIGNAL(updateInformation(int,qulonglong,const QString&)),
                    this,SLOT(updateInformation(int,qulonglong,const QString&)));
            connect(ownInformation,SIGNAL(updateInformationSuccessful(int,qulonglong,const QString&)),
                    this,SLOT(updateInformationSuccessful(int,qulonglong,const QString&)));

            connect(ownInformation,SIGNAL(updateImage(int,qulonglong,const QPixmap&,const QString&)),
                    this,SLOT(updateImage(int,qulonglong,const QPixmap&,const QString&)));
            connect(ownInformation,SIGNAL(updateImageSuccessful(int,qulonglong,const QPixmap&)),
                    this,SLOT(updateImageSuccessful(int,qulonglong,const QPixmap&)));
        }
    }
    else if(type == delFriendType){
        friends->delWidget(friendId);
    }
    else if(type == searchFriendsOfGroupType){
        ChatGroup *group = existChatGroup(groupId);
        if(group != nullptr){
            if(friendImage.isNull()){
                QPixmap image(":/headportrait/head.jpg");
                group->addFriend(friendId,image,friendName,friendSign);
            }
            else group->addFriend(friendId,friendImage,friendName,friendSign);
        }
    }
}

void Homepage::socketGroup(int type, qulonglong groupId,
                           const QString &groupName, const QPixmap &groupImage)
{
    if(type == searchGroupType){
        if(groupImage.isNull()) {
            QPixmap image(":/headportrait/head2.jpg");
            search->receiveGroup(groupId,image,groupName);
        }
        else search->receiveGroup(groupId,groupImage,groupName);
    }
    else if(type == createGroupType || type == searchGroupsType || type == addGroupType){
        if(groupImage.isNull()) {
            QPixmap image(":/headportrait/head2.jpg");
            groups->addWidget(groupId,image,groupName);
        }
        else groups->addWidget(groupId,groupImage,groupName);
    }
    else if(type == delGroupType){
        groups->delWidget(groupId);
    }
}

void Homepage::socketChat(int type, qulonglong sendId, QString sendName, const QString &msg,
                          qulonglong groupId, QString groupName)
{
    if(type == chatFriendType){
        ChatFriend *chat = existChatFriend(sendId);
        if(chat != nullptr){
            toping(chat);
            chat->append(type,msg);
            return ;
        }
        QPixmap image(":/headportrait/head.jpg");
        ChatFriend *newChat = newChatFriend(sendId,image,sendName);
        toping(newChat);
        newChat->append(type,msg);
    }
    else if(type == chatGroupType){
        ChatGroup *chat = existChatGroup(groupId);
        if(chat != nullptr){
            toping(chat);
            chat->append(sendId,sendName,type,msg);
            return ;
        }
        QPixmap image(":/headportrait/head2.jpg");
        ChatGroup *newChat = newChatGroup(groupId,image,groupName);
        socket->getFriendsOfGroup(groupId);
        toping(newChat);
        newChat->append(sendId,sendName,type,msg);
    }
}

void Homepage::on_pushButton_clicked()
{
    ownInformation->show();
}

void Homepage::on_pushButton_2_clicked()
{
    search->show();
}

void Homepage::on_pushButton_3_clicked()
{
    create->show();
}

void Homepage::quit()
{
    socketThread->setStop();
    socket->close();
    emit toLoginQuit();
}

void Homepage::offline()
{
    if(!socket->isValid()) {qDebug()<<"已经离线";return ;}
    socketThread->setStop();
    socket->close();
}

void Homepage::online()
{
    if(socket->isValid()) {qDebug()<<"已经在线";return ;}
    socketThread->login();
    socketThread->start();
}

ChatFriend *Homepage::existChatFriend(qulonglong friendId)
{
    auto ite = chatFriend->find(friendId);
    if(ite != chatFriend->end()) return ite.value();
    return nullptr;
}

ChatGroup *Homepage::existChatGroup(qulonglong groupId)
{
    auto ite = chatGroup->find(groupId);
    if(ite != chatGroup->end()) return ite.value();
    return nullptr;
}

Information *Homepage::existFriendInformation(qulonglong friendId)
{
    auto ite = friendInformation->find(friendId);
    if(ite != friendInformation->end()) return ite.value();
    return nullptr;
}

Information *Homepage::existGroupInformation(qulonglong groupId)
{
    auto ite = groupInformation->find(groupId);
    if(ite != groupInformation->end()) return ite.value();
    return nullptr;
}

ChatFriend *Homepage::newChatFriend(qulonglong friendId, const QPixmap &friendImage, const QString &friendName)
{
    ChatFriend *newChat = new ChatFriend(friendId,friendImage,friendName);
    connect(newChat,SIGNAL(send(int,qulonglong,const QString&,const QString&)),
            this,SLOT(sendMsg(int,qulonglong,const QString&,const QString&)));
    connect(newChat,SIGNAL(removeThis(qulonglong)),this,SLOT(removeChatFriend(qulonglong)));
    chatFriend->insert(friendId,newChat);
    return newChat;
}

ChatGroup *Homepage::newChatGroup(qulonglong groupId, const QPixmap &groupImage, const QString &groupName)
{
    ChatGroup *newChat = new ChatGroup(groupId,groupImage,groupName);
    connect(newChat,SIGNAL(send(int,qulonglong,const QString&,const QString&)),
            this,SLOT(sendMsg(int,qulonglong,const QString&,const QString&)));
    connect(newChat,SIGNAL(removeThis(qulonglong)),this,SLOT(removeChatGroup(qulonglong)));
    connect(newChat,SIGNAL(toHomeShowChatFriend(MyWidget*)),this,SLOT(showChatFriend(MyWidget*)));
    connect(newChat,SIGNAL(toHomeShowInformation(bool,MyWidget*)),this,SLOT(showInformation(bool,MyWidget*)));
    chatGroup->insert(groupId,newChat);
    return newChat;
}

Information *Homepage::newInformation(qulonglong id, const QPixmap &image, const QString &name, const QString &sign, bool editabled)
{
    Information *newInfor = new Information(id,image,name,sign,editabled);
    connect(newInfor,SIGNAL(removeThis(bool,qulonglong)),this,SLOT(removeInformation(bool,qulonglong)));
    if(editabled){
        connect(newInfor,SIGNAL(updateInformation(int,qulonglong,const QString&)),
                this,SLOT(updateInformation(int,qulonglong,const QString&)));
        connect(newInfor,SIGNAL(updateInformationSuccessful(int,qulonglong,const QString&)),
                this,SLOT(updateInformationSuccessful(int,qulonglong,const QString&)));

        connect(newInfor,SIGNAL(updateImage(int,qulonglong,const QPixmap&,const QString&)),
                this,SLOT(updateImage(int,qulonglong,const QPixmap&,const QString&)));
        connect(newInfor,SIGNAL(updateImageSuccessful(int,qulonglong,const QPixmap&)),
                this,SLOT(updateImageSuccessful(int,qulonglong,const QPixmap&)));
        groupInformation->insert(id,newInfor);
    }
    else{
        friendInformation->insert(id,newInfor);
    }
    return newInfor;
}

void Homepage::imageProcessing(QPixmap &result, const QPixmap &pixmap)
{
    QSize size(60,60);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);//抗锯齿
    painter.setRenderHint(QPainter::SmoothPixmapTransform);//平滑像素变换
    painter.fillRect(0,0,size.width(),size.height(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    painter.drawRoundedRect(0,0,size.width(),size.height(),size.width()/2,size.height()/2);
    result = pixmap.scaled(size);
    result.setMask(mask);
}

void Homepage::toping(QWidget *object)
{
    if(object->isHidden()){
        object->show();
        return ;
    }
    Qt::WindowStates winStatus = Qt::WindowNoState;
    if(windowState() & Qt::WindowMaximized) winStatus = Qt::WindowMaximized;
    object->setWindowState(Qt::WindowActive | winStatus);
    object->activateWindow();
    object->raise();
}

void Homepage::tips(const QString &msg, QPoint pos)
{
    QMessageBox messageBox(QMessageBox::NoIcon,"提示!",msg,QMessageBox::Ok);
    messageBox.move(pos);
    QApplication::setQuitOnLastWindowClosed(false);
    messageBox.exec();
    QApplication::setQuitOnLastWindowClosed(true);
}

void Homepage::connectSocketSignals(Socket *sock)
{
    sock->setSocketOption(QAbstractSocket::KeepAliveOption,true);
    connect(sock,SIGNAL(connected()),this,SLOT(connection()));
    connect(sock,SIGNAL(disconnected()),this,SLOT(disconnection()));
    connect(sock,SIGNAL(toUiUpdate(int,QString,qulonglong)),
            this,SLOT(socketUpdate(int,QString,qulonglong)));
    connect(sock,SIGNAL(toHomepageFriend(int, qulonglong,const QString&,const QString&,const QPixmap&, qulonglong)),
            this,SLOT(socketFriend(int, qulonglong,const QString&,const QString&,const QPixmap&, qulonglong)));
    connect(sock,SIGNAL(toHomepageGroup(int,qulonglong,const QString&,const QPixmap&)),
            this,SLOT(socketGroup(int,qulonglong,const QString&,const QPixmap&)));
    connect(sock,SIGNAL(toHomepageChat(int,qulonglong,QString,const QString&,qulonglong,QString)),
            this,SLOT(socketChat(int,qulonglong,QString,const QString&,qulonglong,QString)));
}


