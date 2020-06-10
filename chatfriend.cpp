#include "chatfriend.h"
#include "ui_chatfriend.h"

#include <QBitmap>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

const int chatFriendType = 9000016;//好友聊天

ChatFriend::ChatFriend(qulonglong friendId, const QPixmap &friendImage, const QString &friendName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatFriend)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::CoverWindow);//无窗口边框
    setWindowIcon(QIcon(friendImage));
    setWindowTitle(friendName);

    setAttribute(Qt::WA_DeleteOnClose);

    _friendId = friendId;
    _friendName = friendName;
    ui->label->setText(_friendName);

    ui->label_2->installEventFilter(this);//最小化
    ui->label_3->installEventFilter(this);//关闭
    ui->label_5->installEventFilter(this);//表情

    emojs = new Emojs;
    connect(emojs,SIGNAL(imgUrl(QString)),this,SLOT(sendEmoj(QString)));//发送表情槽
    emojs->hide();
}

ChatFriend::~ChatFriend()
{
    delete ui;
    delete emojs;
}

void ChatFriend::mousePressEvent(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton){
        offSet = event->globalPos()-this->pos();
        emojs->hide();
    }
}

void ChatFriend::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        this->move(event->globalPos()-offSet);//移动主窗体位置
    }
}

bool ChatFriend::eventFilter(QObject *object, QEvent *e)
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
            removeThis(_friendId);
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

void ChatFriend::append(int type, const QString &msg)
{
    if(type == chatFriendType){
        ui->textBrowser->append(_friendName+":<br>"+msg+"<br>");
        ui->textBrowser->setAlignment(Qt::AlignLeft);
        ui->textBrowser->forward();
    }
}

void ChatFriend::on_pushButton_clicked()
{
    removeThis(_friendId);
    close();
}

void ChatFriend::on_pushButton_2_clicked()
{
    QString text = ui->textEdit->toPlainText();
    ui->textEdit->clear();
    if(text == "") return ;
    ui->textBrowser->append(":<br>"+text+"<br>");
    ui->textBrowser->setAlignment(Qt::AlignRight);
    ui->textBrowser->forward();
    emit send(chatFriendType,_friendId,_friendName,text);
}

void ChatFriend::sendEmoj(QString emoj)
{
    QString s = "<img src="+emoj+">";
    ui->textBrowser->append(":<br>"+s+"<br>");
    ui->textBrowser->setAlignment(Qt::AlignRight);
    ui->textBrowser->forward();
    emit send(chatFriendType,_friendId,_friendName,s);
}


