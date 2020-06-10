#include "mywidget.h"
#include "ui_mywidget.h"

#include <QAction>
#include <QBitmap>
#include <QPainter>
#include <QPoint>

MyWidget::MyWidget(int width, int height, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);//隐藏窗口状态栏和图标
    _width = width;
    _height = height;

    setFixedSize(_width,_height);

    QSize headSize = QSize(int(_height*5/7),int(_height*5/7));
    QPoint headPoint = QPoint(int(_height*2/7),int(_height*1/7));
    ui->label->setMinimumSize(headSize);
    ui->label->setMaximumSize(headSize);
    ui->label->move(headPoint);
    ui->label->show();

    QSize signSize = QSize(int(_width-_height*11/7),int(_height*3/14));
    QPoint signPoint = QPoint(int(_height*9/7),int(_height*8/14));
    ui->label_3->setMinimumSize(signSize);
    ui->label_3->setMaximumSize(signSize);
    ui->label_3->move(signPoint);
    ui->label_3->setFont(QFont("宋体",9));
    ui->label_3->hide();

    _hasSign = true;
    setHasSign(false);//默认无签名
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::init(qulonglong id, const QPixmap &pixmap,
                    const QString &name, const QString &sign)
{
    _id = id;
    _pixmap = pixmap;
    setName(name);
    setSign(sign);
    setPixmap(pixmap);
}

void MyWidget::setHasSign(const bool &hasSign)
{
    if(hasSign == _hasSign) return ;
    _hasSign = hasSign;
    change();
}

void MyWidget::setPixmap(const QPixmap &pixmap)
{
    QPixmap result;
    imageProcessing(result,pixmap);
    ui->label->setPixmap(result);
}

QPixmap MyWidget::getPixmap() const
{
    return _pixmap;
}

qulonglong MyWidget::getId() const
{
    return _id;
}

void MyWidget::setName(const QString &name)
{
    ui->label_2->setText(name);
}

QString MyWidget::getName() const
{
    return ui->label_2->text();
}

void MyWidget::setSign(const QString &sign)
{
    if(_hasSign) ui->label_3->setText(sign);
}

QString MyWidget::getSign() const
{
    if(_hasSign) return ui->label_3->text();
    else return "";
}

void MyWidget::change()
{
    if(_hasSign){
        changeNameLabel(2.0/7);
        ui->label_3->show();
    }
    else{
        changeNameLabel(4.0/7);
        ui->label_3->hide();
    }
}

void MyWidget::changeNameLabel(double factor)
{
    QSize nameSize = QSize(int(_width-_height*11/7),int(_height*factor));
    QPoint namePoint = QPoint(int(_height*9/7),int(_height*3.0/14));
    ui->label_2->setMinimumSize(nameSize);
    ui->label_2->setMaximumSize(nameSize);
    ui->label_2->move(namePoint);
    ui->label_2->setFont(QFont("黑体",_height*factor-9-(!_hasSign*10)));
    ui->label_2->show();
}

void MyWidget::imageProcessing(QPixmap &result, const QPixmap &pixmap)
{
    QSize size(_height*5/7,_height*5/7);
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

