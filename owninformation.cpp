#include "owninformation.h"
#include "ui_owninformation.h"

#include <QBitmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QBuffer>

const QByteArray requestSuccessful = "Request successful!";

const int modifyOwnPasswordType = 9000002;//修改个人密码
const int modifyOwnNameType = 9000003;//修改个人昵称
const int modifyOwnSignType = 9000004;//修改个人签名
const int modifyOwnImageType = 9000018;//修改个人签名

OwnInformation::OwnInformation(qulonglong ownId,const QPixmap &ownImage, const QString &ownName,
                               const QString &ownSign,const QString &ownPassword, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OwnInformation)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//隐藏窗口状态栏和窗口图标
    setWindowIcon(QIcon(ownImage));//任务栏图标
    setWindowTitle(ownName);

    returnInformation();//默认信息界面

    _ownId = ownId;
    _ownImage = ownImage;
    _ownName = ownName;
    _ownSign = ownSign;
    _ownPassword = ownPassword;

    ui->label_8->setText(QString::number(_ownId));
    QPixmap result;
    imageProcessing(result,_ownImage);
    ui->label_4->setPixmap(result);
    ui->lineEdit->setText(_ownName);
    ui->lineEdit_2->setText(_ownSign);

    ui->lineEdit_3->setMaxLength(12);//最长密码
    ui->lineEdit_3->setPlaceholderText(QStringLiteral("旧密码"));//占位符
    ui->lineEdit_3->setEchoMode(QLineEdit::EchoMode::Password);//回声密码形式

    ui->lineEdit_4->setMaxLength(12);//最长密码
    ui->lineEdit_4->setPlaceholderText(QStringLiteral("新密码"));//占位符
    ui->lineEdit_4->setEchoMode(QLineEdit::EchoMode::Password);//回声密码形式

    ui->label_2->installEventFilter(this);//最小化
    ui->label_3->installEventFilter(this);//关闭
}

OwnInformation::~OwnInformation()
{
    delete ui;
}

void OwnInformation::mousePressEvent(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton){
        offSet = event->globalPos()-this->pos();
    }
}

void OwnInformation::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        this->move(event->globalPos()-offSet);//移动主窗体位置
    }
}

bool OwnInformation::eventFilter(QObject *object, QEvent *e)
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

void OwnInformation::receive(int type, const QString &msg)
{
    if(type == modifyOwnNameType){
        if(msg == requestSuccessful){
            _ownName = ui->lineEdit->text();
            emit updateInformationSuccessful(modifyOwnNameType,_ownId,_ownName);
            tips("昵称修改成功!");
            return ;
        }
        ui->lineEdit->setText(_ownName);
        tips("昵称修改失败!");
    }
    else if(type == modifyOwnSignType){
        if(msg == requestSuccessful){
            _ownSign = ui->lineEdit_2->text();
            emit updateInformationSuccessful(modifyOwnSignType,_ownId,_ownSign);
            tips("签名修改成功!");
            return ;
        }
        ui->lineEdit_2->setText(_ownSign);
        tips("签名修改失败!");
    }
    else if(type == modifyOwnPasswordType){
        if(msg == requestSuccessful){
            _ownPassword = ui->lineEdit_4->text();
            emit updateInformationSuccessful(modifyOwnPasswordType,_ownId,_ownPassword);
            returnInformation();
            tips("密码修改成功!");
            return ;
        }
        ui->lineEdit_3->clear();
        ui->lineEdit_4->clear();
        tips("密码修改失败!");
    }
    else if(type == modifyOwnImageType){
        if(msg == requestSuccessful){
            _ownImage = *ui->label_4->pixmap();
            emit updateImageSuccessful(modifyOwnImageType,_ownId,_ownImage);
            tips("头像修改成功!");
            return ;
        }
        ui->label_4->setPixmap(_ownImage);
        tips("头像修改失败!");
    }
}

void OwnInformation::on_lineEdit_returnPressed()
{
    if(ui->lineEdit->text() != "" && ui->lineEdit->text() != _ownName)
        emit updateInformation(modifyOwnNameType,_ownId,ui->lineEdit->text());
}

void OwnInformation::on_lineEdit_2_returnPressed()
{
    if(ui->lineEdit_2->text() != _ownSign)
        emit updateInformation(modifyOwnSignType,_ownId,ui->lineEdit_2->text());
}

void OwnInformation::on_pushButton_clicked()
{
    //头像
    QString file = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                             "C:/Users/Administrator/Pictures",
                                                             tr("Images (*.jpg)"));
    if(file != ""){
        QPixmap original(file);
        QPixmap pixmap;
        imageProcessing(pixmap,original);
        ui->label_4->setPixmap(pixmap);

        QMessageBox messageBox(QMessageBox::NoIcon,"确认!","确定修改头像吗?",QMessageBox::Yes|QMessageBox::No);
        messageBox.move(x()+135,y()+90);
        QApplication::setQuitOnLastWindowClosed(false);
        int result = messageBox.exec();
        QApplication::setQuitOnLastWindowClosed(true);
        switch (result) {
        case QMessageBox::Yes:
            emit updateImage(modifyOwnImageType,_ownId,original.scaled(QSize(100,100)),file.right(3));
            break;
        case QMessageBox::No:
            ui->label_4->setPixmap(_ownImage);
            break;
        default:break;
        }
    }
}

void OwnInformation::on_pushButton_2_clicked()
{
    change();
}

void OwnInformation::on_pushButton_3_clicked()
{
    if(ui->lineEdit_3->text() == "" || ui->lineEdit_4->text() == ""){
        tips("新旧密码不能为空");
        return ;
    }
    if(ui->lineEdit_3->text() != _ownPassword){
        tips("原密码不正确!");
        return ;
    }
    emit updateInformation(modifyOwnPasswordType,_ownId,ui->lineEdit_4->text());
}

void OwnInformation::change()
{
    if(which) returnInformation();
    else toUpdatePassword();
}

void OwnInformation::toUpdatePassword()
{
    which = true;
    ui->lineEdit->hide();//昵称隐藏
    ui->lineEdit_2->hide();//签名隐藏
    ui->lineEdit_3->show();//旧密码显示
    ui->lineEdit_4->show();//新密码显示
    ui->pushButton_3->show();//提交密码显示
    ui->label_5->hide();//账号标签隐藏
    ui->label_6->hide();//昵称标签隐藏
    ui->label_7->hide();//签名标签隐藏
    ui->label_8->hide();//账号框隐藏
    ui->pushButton_2->setText("返回信息");
}

void OwnInformation::returnInformation()
{
    which = false;
    ui->lineEdit->show();//昵称显示
    ui->lineEdit_2->show();//签名显示
    ui->lineEdit_3->clear();//旧密码清空
    ui->lineEdit_4->clear();//新密码清空
    ui->lineEdit_3->hide();//旧密码隐藏
    ui->lineEdit_4->hide();//新密码隐藏
    ui->pushButton_3->hide();//提交密码隐藏
    ui->label_5->show();//账号标签显示
    ui->label_6->show();//昵称标签显示
    ui->label_7->show();//签名标签显示
    ui->label_8->show();//账号框显示
    ui->pushButton_2->setText("修改密码");
}

void OwnInformation::tips(const QString &msg)
{
    QMessageBox messageBox(QMessageBox::NoIcon,"提示!",msg,QMessageBox::Ok);
    messageBox.move(x()+135,y()+90);
    QApplication::setQuitOnLastWindowClosed(false);
    messageBox.exec();
    QApplication::setQuitOnLastWindowClosed(true);
}

void OwnInformation::imageProcessing(QPixmap &result, const QPixmap &pixmap)
{
    QSize size(100,100);
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
