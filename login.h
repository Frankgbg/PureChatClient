#ifndef LOGIN_H
#define LOGIN_H

#include "homepage.h"

#include <QPropertyAnimation>
#include <QSystemTrayIcon>
#include <QTcpSocket>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    void mousePressEvent(QMouseEvent *event) override;//鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;//鼠标移动事件
    bool eventFilter(QObject *object, QEvent *e) override;//事件过滤器

public slots:
    void socketId(qulonglong id);//接收注册账号
    void socketUpdate(int type, QString msg);//接收登录验证结果
    void quit();//退出

private slots:
    void onSysTrayIconActived(QSystemTrayIcon::ActivationReason reason);//任务栏图标活动
    void on_pushButton_clicked();//登录

private:
    void hideControl();//隐藏控件
    void showControl();//显示控件
    void change();//改变登录注册状态
    void signUp();//注册状态
    void returnLogin();//返回登录状态
    void toping(QWidget *object);

private:
    Ui::Login *ui;
    Homepage *homepage;
    QPoint offSet; //鼠标相对于窗体的位置
    QPropertyAnimation *animation;//头像动画
    QSystemTrayIcon *sysTrayIcon;//任务栏图标
    Socket *socket;
    qulonglong ownId;
    QString ownPassword;
    bool which;
};

#endif // LOGIN_H
