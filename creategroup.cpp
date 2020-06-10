#include "creategroup.h"
#include "ui_creategroup.h"

#include <QMouseEvent>

CreateGroup::CreateGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateGroup)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//隐藏程序框
    setWindowIcon(QIcon(":/additional/create_128px.ico"));//任务栏图标

    ui->lineEdit->setMaxLength(20);

    ui->label_2->installEventFilter(this);//最小化
    ui->label_3->installEventFilter(this);//关闭
}

CreateGroup::~CreateGroup()
{
    delete ui;
}

void CreateGroup::mousePressEvent(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton){
        offSet = event->globalPos()-this->pos();
    }
}

void CreateGroup::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        this->move(event->globalPos()-offSet);//移动主窗体位置
    }
}

bool CreateGroup::eventFilter(QObject *object, QEvent *e)
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
    return false;
}

void CreateGroup::on_pushButton_clicked()
{
    if(ui->lineEdit->text() == "") return ;
    emit createGroup(ui->lineEdit->text());
    ui->lineEdit->clear();
}

