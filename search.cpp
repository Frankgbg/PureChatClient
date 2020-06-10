#include "search.h"
#include "ui_search.h"

#include <QBitmap>
#include <QMouseEvent>
#include <QPainter>

const int searchFriendType = 9000006;//查找好友
const int addFriendType = 9000007;//添加好友
const int searchGroupType = 9000010;//查找群
const int addGroupType = 9000011;//添加群

Search::Search(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Search)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//隐藏程序框
    setWindowIcon(QIcon(":/additional/search_128px.ico"));//任务栏图标

    searchFriend();//默认找人

    ui->label_2->installEventFilter(this);//最小化
    ui->label_3->installEventFilter(this);//关闭
    ui->label_4->installEventFilter(this);//找人
    ui->label_5->installEventFilter(this);//找群
}

Search::~Search()
{
    delete ui;
}

void Search::mousePressEvent(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton){
        offSet = event->globalPos()-this->pos();
    }
}

void Search::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        this->move(event->globalPos()-offSet);//移动主窗体位置
    }
}

bool Search::eventFilter(QObject *object, QEvent *e)
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
            hide();
            return true;
        }
    }
    else if(object == ui->label_4){//找人
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){
            searchFriend();
            return true;
        }
    }
    else if(object == ui->label_5){//找群
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){
            searchGroup();
            return true;
        }
    }
    return false;
}

void Search::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}

void Search::on_pushButton_clicked()
{
    QString id = ui->lineEdit->text();
    if(id == "") return ;

    bool isSuccess;
    inputId = id.toULongLong(&isSuccess,10);
    if(!isSuccess){
        tips("请输入数字!");
        return ;
    }

    if(which){//找群
        if(inputId < 1000 || inputId > 999999999){
            tips("群号输入错误!");
            return ;
        }
        emit updateSearch(searchGroupType,inputId);
    }
    else{//找好友
        if(inputId < 100000 || inputId > 9999999999){
            tips("账号输入错误!");
            return ;
        }
        emit updateSearch(searchFriendType,inputId);
    }
}

void Search::on_pushButton_2_clicked()
{
    if(which) emit updateSearch(addGroupType,recId);//加群
    else emit updateSearch(addFriendType,recId);//加好友
}

void Search::receiveFriend(qulonglong friendId, const QPixmap &image,
                           const QString &friendName, const QString &friendSign)
{
    recId = friendId;
    if(recId != inputId){
        if(recId == 0) tips("空账号.");
        else tips("重新查找.");
        return ;
    }
    setInformation(image,friendName,friendSign);
}

void Search::receiveGroup(qulonglong groupId, const QPixmap &image, const QString &groupName)
{
    recId = groupId;
    if(recId != inputId){
        if(recId == 0) tips("空账号.");
        else tips("重新查找.");
        return ;
    }
    setInformation(image,groupName,"");
}

void Search::setInformation(const QPixmap &image, const QString &name, const QString &sign)
{
    QPixmap _image;
    imageProcessing(_image,image);
    ui->label_7->setPixmap(_image);
    ui->label_8->setText(name);
    ui->label_8->setAlignment(Qt::AlignLeft);
    ui->label_8->setStyleSheet(QString("#label_8{font-weight:bold;font-size:18px;}\n"
                                       "#label_8:hover{color:black;}"));
    ui->label_9->setText(sign);
    if(which) ui->pushButton_2->setText("+群");
    else ui->pushButton_2->setText("+好友");
    showAll();
}

void Search::tips(const QString &msg)
{
    hideAll();
    ui->label_8->setText(msg);
    ui->label_8->setAlignment(Qt::AlignHCenter);
    ui->label_8->setStyleSheet(QString("#label_8{font-weight:normal;font-size:16px;}\n"
                                       "#label_8:hover{color:black;}"));
    ui->label_8->show();
}

void Search::searchFriend()
{
    which = false;
    ui->label_4->setStyleSheet(QString("#label_4{font-weight:bold;font-size:16px;color:white;}\n"
                                       "#label_4:hover{color:rgb(255,255,255);}"));
    ui->label_5->setStyleSheet(QString("#label_5{font-weight:bold;font-size:16px;color:black;}\n"
                                       "#label_5:hover{color:rgb(255,255,255);}"));
    hideAll();
}

void Search::searchGroup()
{
    which = true;
    ui->label_4->setStyleSheet(QString("#label_4{font-weight:bold;font-size:16px;color:black;}\n"
                                       "#label_4:hover{color:rgb(255,255,255);}"));
    ui->label_5->setStyleSheet(QString("#label_5{font-weight:bold;font-size:16px;color:white;}\n"
                                       "#label_5:hover{color:rgb(255,255,255);}"));
    hideAll();
}

void Search::showAll()
{
    ui->label_7->show();
    ui->label_8->show();
    ui->label_9->show();
    ui->pushButton_2->show();
}

void Search::hideAll()
{
    ui->label_7->hide();
    ui->label_8->hide();
    ui->label_9->hide();
    ui->pushButton_2->hide();
}

void Search::imageProcessing(QPixmap &result, const QPixmap &image)
{
    QSize size(74,74);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);//抗锯齿
    painter.setRenderHint(QPainter::SmoothPixmapTransform);//平滑像素变换
    painter.fillRect(0,0,size.width(),size.height(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    painter.drawRoundedRect(0,0,size.width(),size.height(),size.width()/2,size.height()/2);
    result = image.scaled(size);
    result.setMask(mask);
}

