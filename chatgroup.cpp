#include "chatgroup.h"
#include "ui_chatgroup.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPixmap>

const int chatGroupType = 9000017;//群聊天

ChatGroup::ChatGroup(qulonglong groupId, const QPixmap &groupImage, const QString &groupName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatGroup)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::CoverWindow);//无窗口边框
    setWindowIcon(groupImage);
    setWindowTitle(groupName);

    setAttribute(Qt::WA_DeleteOnClose);

    _groupId = groupId;
    _groupName = groupName;
    ui->label->setText(_groupName);

    ui->label_2->installEventFilter(this);//最小化
    ui->label_3->installEventFilter(this);//关闭
    ui->label_5->installEventFilter(this);//表情

    emojs = new Emojs;
    connect(emojs,SIGNAL(imgUrl(QString)),this,SLOT(sendEmoj(QString)));//发送表情槽
    emojs->hide();

    QVBoxLayout *layout = new QVBoxLayout;
    ui->widget_5->setLayout(layout);
    friends = new MyListWidget(170,49,false);
    layout->addWidget(friends);
    connect(friends,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(showChatFriend(QListWidgetItem*)));
    connect(friends,SIGNAL(showInformation(MyWidget*)),this,SLOT(showInformation(MyWidget*)));
}

ChatGroup::~ChatGroup()
{
    delete ui;
    delete emojs;
    delete friends;
}

void ChatGroup::mousePressEvent(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton){
        offSet = event->globalPos()-this->pos();
        emojs->hide();
    }
}

void ChatGroup::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        this->move(event->globalPos()-offSet);//移动主窗体位置
    }
}

bool ChatGroup::eventFilter(QObject *object, QEvent *e)
{
    if(object == ui->label_2){//最小化
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            showMinimized();
            return true;
        }
    }
    else if(object == ui->label_3){//关闭
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){
            removeThis(_groupId);
            close();
            return true;
        }
    }
    else if(object == ui->label_5){//表情
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){
            emojs->move(x()+5,y()+ui->textBrowser->height()-50);
            emojs->show();
            return true;
        }
    }
    return false;
}

void ChatGroup::append(qulonglong sendId, QString sendName, int type, const QString &msg)
{
    if(type == chatGroupType){
        ui->textBrowser->append(sendName+":<br>"+msg+"<br>");
        ui->textBrowser->setAlignment(Qt::AlignLeft);
        ui->textBrowser->forward();
    }
}

void ChatGroup::addFriend(qulonglong friendId, const QPixmap &friendImage, const QString &friendName, const QString &friendSign)
{
    friends->addWidget(friendId,friendImage,friendName,friendSign);
}

void ChatGroup::showInformation(MyWidget *currentWidget)
{
    emit toHomeShowInformation(false,currentWidget);
}

void ChatGroup::on_pushButton_clicked()
{
    removeThis(_groupId);
    close();
}

void ChatGroup::on_pushButton_2_clicked()
{
    QString text = ui->textEdit->toPlainText();
    ui->textEdit->clear();
    if(text == "") return ;
    ui->textBrowser->append(":<br>"+text+"<br>");
    ui->textBrowser->setAlignment(Qt::AlignRight);
    ui->textBrowser->forward();
    emit send(chatGroupType,_groupId,_groupName,text);
}

void ChatGroup::sendEmoj(QString emoj)
{
    QString s = "<img src="+emoj+">";
    ui->textBrowser->append(":<br>"+s+"<br>");
    ui->textBrowser->setAlignment(Qt::AlignRight);
    ui->textBrowser->forward();
    emit send(chatGroupType,_groupId,_groupName,s);
}

void ChatGroup::showChatFriend(QListWidgetItem *chatFriend)
{
    emit toHomeShowChatFriend(static_cast<MyWidget*>(friends->itemWidget(chatFriend)));
}

