#include "login.h"
#include "ui_login.h"

#include <QFile>
#include <QMovie>
#include <QDebug>
#include <QDesktopServices>
#include <QMouseEvent>
#include <qjsondocument.h>
#include <QMessageBox>
#include <QProcess>
#include <synchapi.h>
#include <QMenu>

const QByteArray successful = "Login successful.";
const QByteArray failed = "Login failed!";
const QByteArray notExist = "Account/Group does not exists!";
const QByteArray logged = "Account already logged in!";
const QByteArray reLogin = "Please login again.";
const QByteArray numberError = "Account/Group number error!";
const QByteArray exist = "Account/Group exists.";

const int signUpType = 9000000;
const int loginType = 9000001;

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);//隐藏窗口状态栏

    setAttribute(Qt::WA_QuitOnClose);

    QMovie *movie = new QMovie(":/login/sky.gif");//背景gif
    ui->label->setMovie(movie);
    movie->start();

    /*任务栏图标*/
    sysTrayIcon = new QSystemTrayIcon(this);
    sysTrayIcon->setIcon(QIcon(":/homepage/chat_72px.ico"));
    sysTrayIcon->show();

    QMenu *menuTray = new QMenu;//图标菜单
    QAction *actQuit = new QAction(tr("退出"), menuTray);
    menuTray->addAction(actQuit);//菜单中动作
    sysTrayIcon->setContextMenu(menuTray);
    sysTrayIcon->setToolTip("PureChat");

    connect(sysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(onSysTrayIconActived(QSystemTrayIcon::ActivationReason)));
    connect(actQuit, SIGNAL(triggered()), this, SLOT(quit()));

    ui->label_4->setStyleSheet("border-image:url(':/login/head.jpg')");//头像

    /*注册事件过滤器*/
    ui->label_2->installEventFilter(this);//最小化
    ui->label_3->installEventFilter(this);//关闭
    ui->label_5->installEventFilter(this);//注册账号

    /*注册账号时昵称框*/
    ui->lineEdit_2->setFont(QFont("楷体",12));//字体
    ui->lineEdit_2->setMaxLength(20);//最长密码
    ui->lineEdit_2->setPlaceholderText(QStringLiteral("昵称"));//占位符
    ui->lineEdit_2->hide();

    which = false;
    /*账号框*/
    ui->comboBox->setEditable(true);
    ui->comboBox->setFont(QFont("楷体",12));//字体
    QLineEdit *lineEdit =  ui->comboBox->lineEdit();
    lineEdit->setMaxLength(10);//最长账号
    lineEdit->setPlaceholderText(QStringLiteral("账号"));//占位符

    /*密码框*/
    ui->lineEdit->setMaxLength(12);//最长密码
    ui->lineEdit->setPlaceholderText(QStringLiteral("密码"));//占位符
    ui->lineEdit->setEchoMode(QLineEdit::EchoMode::Password);//回声密码形式
    connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(on_pushButton_clicked()));

    /*登录按钮*/
    ui->pushButton->setText("登录");
    ui->pushButton->setShortcut(Qt::Key_Return);//快捷键

    /*登录时头像动画*/
    animation=new QPropertyAnimation(ui->label_4, "geometry",this);
    animation->setDuration(400);
    animation->setStartValue(QRect(30, 158, 84, 84));
    animation->setEndValue(QRect(158, 158, 84, 84));

    socket = nullptr;
    homepage = nullptr;
}

Login::~Login()
{
    delete ui;
    if(homepage != nullptr){
        delete homepage;
        homepage = nullptr;
    }
    if(socket != nullptr) {
        delete socket;
        socket = nullptr;
    }
}

void Login::mousePressEvent(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton){
        offSet = event->globalPos()-this->pos();
    }
}

void Login::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        this->move(event->globalPos()-offSet);//移动主窗体位置
    }
}

bool Login::eventFilter(QObject *object, QEvent *e)
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
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            quit();
            return true;
        }
    }
    else if(object == ui->label_5){//注册
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            change();
            return true;
        }
    }
    return false;
}

void Login::socketId(qulonglong id)
{
    QMessageBox messageBox(QMessageBox::NoIcon,"信息","注册到账号:"+QString::number(id),QMessageBox::Ok);
    QApplication::setQuitOnLastWindowClosed(false);
    messageBox.exec();
    QApplication::setQuitOnLastWindowClosed(true);
    returnLogin();
    ui->comboBox->lineEdit()->setText(QString::number(id));
    socket->close();
    socket = nullptr;
}

void Login::socketUpdate(int type, QString msg)
{
    if(type == loginType){
        if(msg == successful){
            disconnect(socket,0,this,0);
            homepage = new Homepage(ownId,ownPassword,socket);
            connect(homepage,SIGNAL(toLoginQuit()),this,SLOT(quit()));
            sysTrayIcon->hide();
            hide();
            socket = nullptr;
            homepage->show();
            return ;
        }
        showControl();
        if(msg == failed) ui->label_6->setText("密码错误!");
        else if(msg == notExist) ui->label_6->setText("账号不存在!");
        else if(msg == logged) ui->label_6->setText("账号已登录!");
    }
    else if(type == signUpType){
        ui->label_6->setText("注册失败!");
    }
    socket->close();
    socket = nullptr;
}

void Login::quit()
{
    this->close();
}

void Login::onSysTrayIconActived(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
        case QSystemTrayIcon::Trigger://单击
        case QSystemTrayIcon::DoubleClick://双击
            toping(this);
            break;
        default:break;
    }
}

void Login::on_pushButton_clicked()
{
    /*注册*/
    if(which){
        if(ui->lineEdit_2->text() == "" || ui->lineEdit->text() == ""){
            ui->label_6->setText("昵称或密码不能为空!");
            return ;
        }
        socket = new Socket;
        connect(socket,SIGNAL(toLoginId(qulonglong)),this,SLOT(socketId(qulonglong)));
        connect(socket,SIGNAL(toUiUpdate(int,QString,qulonglong)),
                this,SLOT(socketUpdate(int,QString)));

        socket->connectToHost("47.96.226.11",61314);

        if(!socket->waitForConnected(300)){
            delete socket;
            socket = nullptr;
            ui->label_6->setText("无法连接到服务器,稍后尝试.");
            return ;
        }
        socket->signUp(ui->lineEdit->text(),ui->lineEdit_2->text());
        return ;
    }
    /*登录*/
    if(ui->comboBox->lineEdit()->text() == "" || ui->lineEdit->text() == ""){
        ui->label_6->setText("账号或密码不能为空!");
    }
    else{
        ownId = ui->comboBox->lineEdit()->text().toULongLong();
        if(ownId == 0 || ownId < 100000 || ownId > 9999999999){
            ui->label_6->setText("账号输入错误");
            return ;
        }
        ownPassword = ui->lineEdit->text();

        hideControl();
        //animation->start();

        socket = new Socket;
        connect(socket,SIGNAL(toUiUpdate(int,QString,qulonglong)),
                this,SLOT(socketUpdate(int,QString)));

        socket->connectToHost("47.96.226.11",61314);

        if(!socket->waitForConnected(300)){
            delete socket;
            socket = nullptr;
            showControl();
            ui->label_6->setText("无法连接到服务器,稍后尝试.");
            return ;
        }
        socket->login(ownId,ownPassword);
    }
}

void Login::hideControl()
{
    ui->comboBox->hide();
    ui->lineEdit->hide();
    ui->pushButton->hide();
    ui->label_6->hide();
    ui->label_5->hide();
}

void Login::showControl()
{
    ui->label_4->move(QPoint(158,103));
    ui->label_4->show();
    ui->comboBox->show();
    ui->lineEdit->show();
    ui->pushButton->show();
    ui->label_6->show();
    ui->label_5->show();
}

void Login::change()
{
    if(which) returnLogin();
    else signUp();
}

void Login::signUp()
{
    which = true;
    ui->label_4->hide();//头像隐藏
    ui->comboBox->lineEdit()->clear();
    ui->comboBox->hide();//账号框隐藏
    ui->lineEdit->clear();//密码框清空
    ui->lineEdit_2->show();//昵称框显示
    ui->label_5->setText("返回");
    ui->pushButton->setText("注册");
}

void Login::returnLogin()
{
    which = false;
    ui->label_4->show();//头像显示
    ui->lineEdit_2->clear();
    ui->lineEdit_2->hide();//昵称框隐藏
    ui->lineEdit->clear();//密码框清空
    ui->comboBox->show();//账号框显示
    ui->label_5->setText("注册账号");
    ui->pushButton->setText("登录");
}

void Login::toping(QWidget *object)
{
    if (object->isHidden()) {
        show();
        return ;
    }
    //置顶
    Qt::WindowStates winStatus = Qt::WindowNoState;
    if(windowState() & Qt::WindowMaximized) winStatus = Qt::WindowMaximized;
    object->setWindowState(Qt::WindowActive | winStatus);
    object->activateWindow();
    object->raise();
}

