#include "information.h"
#include "ui_information.h"

#include <QBitmap>
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>

const QByteArray requestSuccessful = "Request successful!";
const int modifyGroupNameType = 9000005;//修改群昵称
const int modifyGroupImageType = 9000019;//修改群头像

Information::Information(qulonglong id, const QPixmap &image, const QString &name,
                         const QString &sign, bool editabled, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Information)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::CoverWindow);//隐藏窗口状态栏和窗口图标
    setWindowIcon(QIcon(image));//任务栏图标
    setWindowTitle(name);

    setAttribute(Qt::WA_DeleteOnClose);

    imageProcessing(_image,image);
    _id = id;
    _name = name;
    _editabled = editabled;

    ui->label_6->setText(QString::number(_id));
    ui->label_4->setPixmap(_image);
    ui->lineEdit->setText(name);
    ui->label_5->setText(sign);
    if(!_editabled) {
        ui->pushButton->hide();
        ui->lineEdit->setFocusPolicy(Qt::NoFocus);
        ui->lineEdit->setStyleSheet(QString("#lineEdit{border:none;background:rgb(235,242,249);font-weight:bold;}\n"));
    }
    else{
        ui->lineEdit->setStyleSheet(QString("#lineEdit{border:none;background:rgb(235,242,249);font-weight:bold;}\n"
                                            "#lineEdit:hover{border:1px solid rgb(135,142,149);;}"));
        ui->lineEdit->setMaxLength(20);
        ui->label_5->hide();
        ui->label_9->hide();
    }

    ui->label_2->installEventFilter(this);//最小化
    ui->label_3->installEventFilter(this);//关闭
}

Information::~Information()
{
    delete ui;
}

void Information::mousePressEvent(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton){
        offSet = event->globalPos()-this->pos();
    }
}

void Information::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        this->move(event->globalPos()-offSet);//移动主窗体位置
    }
}

bool Information::eventFilter(QObject *object, QEvent *e)
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
            removeThis(_editabled,_id);
            close();
            return true;
        }
    }
    return false;
}

void Information::receive(int type, const QString &msg)
{
    if(type == modifyGroupNameType){
        if(msg == requestSuccessful){
            _name = ui->lineEdit->text();
            emit updateInformationSuccessful(modifyGroupNameType,_id,_name);
            tips("群昵称修改成功.");
            return ;
        }
        ui->lineEdit->setText(_name);
        tips("群昵称修改失败!");
    }
    else if(type == modifyGroupImageType){
        if(msg == requestSuccessful){
            _image = *ui->label_4->pixmap();
            emit updateImageSuccessful(modifyGroupImageType,_id,_image);
            tips("群头像修改成功.");
            return ;
        }
        ui->label_4->setPixmap(_image);
        tips("群头像修改失败!");
    }
}

void Information::on_lineEdit_returnPressed()
{
    if(_editabled){
        if(ui->lineEdit->text() != ""){
            emit updateInformation(modifyGroupNameType,_id,ui->lineEdit->text());
        }
        else{
            ui->lineEdit->setText(_name);
        }
    }
}

void Information::on_pushButton_clicked()
{
    //头像
    QString file = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                             "C:/Users/Administrator/Pictures",
                                                             tr("Images (*.png *.jpg *.bmp)"));
    if(file != ""){
        QPixmap original(file);
        QPixmap pixmap;
        imageProcessing(pixmap,original);
        ui->label_4->setPixmap(pixmap);

        QMessageBox messageBox(QMessageBox::NoIcon,"确认!","确定修改群头像吗?",QMessageBox::Yes|QMessageBox::No);
        messageBox.move(x()+135,y()+90);
        QApplication::setQuitOnLastWindowClosed(false);
        int result = messageBox.exec();
        QApplication::setQuitOnLastWindowClosed(true);
        switch (result) {
        case QMessageBox::Yes:
            emit updateImage(modifyGroupImageType,_id,original.scaled(QSize(100,100)),file.right(3));
            break;
        case QMessageBox::No:
            ui->label_4->setPixmap(_image);
            break;
        default:break;
        }
    }
}

void Information::tips(const QString &msg)
{
    QMessageBox messageBox(QMessageBox::NoIcon,"提示!",msg,QMessageBox::Ok);
    messageBox.move(this->pos()+QPoint(135,90));
    QApplication::setQuitOnLastWindowClosed(false);
    messageBox.exec();
    QApplication::setQuitOnLastWindowClosed(true);
}

void Information::imageProcessing(QPixmap &result, const QPixmap &pixmap)
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

