#include "emojs.h"
#include "ui_emojs.h"

#include <QMouseEvent>

Emojs::Emojs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Emojs)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint|Qt::X11BypassWindowManagerHint);

    ui->label->installEventFilter(this);
    ui->label_2->installEventFilter(this);
    ui->label_3->installEventFilter(this);
    ui->label_4->installEventFilter(this);
    ui->label_5->installEventFilter(this);
    ui->label_6->installEventFilter(this);
    ui->label_7->installEventFilter(this);
    ui->label_8->installEventFilter(this);
    ui->label_9->installEventFilter(this);
    ui->label_10->installEventFilter(this);
    ui->label_11->installEventFilter(this);
    ui->label_12->installEventFilter(this);
    ui->label_13->installEventFilter(this);
    ui->label_14->installEventFilter(this);
    ui->label_15->installEventFilter(this);
    ui->label_16->installEventFilter(this);
    ui->label_17->installEventFilter(this);
    ui->label_18->installEventFilter(this);
}

Emojs::~Emojs()
{
    delete ui;
}

bool Emojs::eventFilter(QObject *object, QEvent *e)
{
    if(object == ui->label){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/cy.png"));
            return true;
        }
    }
    else if(object == ui->label_2){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/dk.png"));
            return true;
        }
    }
    else if(object == ui->label_3){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/dy.png"));
            return true;
        }
    }
    else if(object == ui->label_4){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/fd.png"));
            return true;
        }
    }
    else if(object == ui->label_5){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/gz.png"));
            return true;
        }
    }
    else if(object == ui->label_6){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/hanx.png"));
            return true;
        }
    }
    else if(object == ui->label_7){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/kb.png"));
            return true;
        }
    }
    else if(object == ui->label_8){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/ll.png"));
            return true;
        }
    }
    else if(object == ui->label_9){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/pz.png"));
            return true;
        }
    }
    else if(object == ui->label_10){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/tp.png"));
            return true;
        }
    }
    else if(object == ui->label_11){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/tx.png"));
            return true;
        }
    }
    else if(object == ui->label_12){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/wq.png"));
            return true;
        }
    }
    else if(object == ui->label_13){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/xjj.png"));
            return true;
        }
    }
    else if(object == ui->label_14){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/xk.png"));
            return true;
        }
    }
    else if(object == ui->label_15){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/xyx.png"));
            return true;
        }
    }
    else if(object == ui->label_16){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/yiw.png"));
            return true;
        }
    }
    else if(object == ui->label_17){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/zj.png"));
            return true;
        }
    }
    else if(object == ui->label_18){
        if(e->type() == QEvent::MouseButtonRelease && \
            (static_cast<QMouseEvent*>(e))->button() == Qt::LeftButton){//鼠标左键释放
            hide();
            emit imgUrl(QString(":/emojs/zk.png"));
            return true;
        }
    }
    return false;
}

