#ifndef SEARCH_H
#define SEARCH_H

#include <QSystemTrayIcon>
#include <QWidget>

namespace Ui {
class Search;
}

class Search : public QWidget
{
    Q_OBJECT
public:
    explicit Search(QWidget *parent = nullptr);
    ~Search();

    void mousePressEvent(QMouseEvent *event) override;//鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;//鼠标移动事件
    bool eventFilter(QObject *object, QEvent *e) override;//事件过滤器

public:
    void receiveFriend(qulonglong friendId, const QPixmap &image, const QString &friendName, const QString &friendSign);//设置接收到好友信息
    void receiveGroup(qulonglong groupId, const QPixmap &image, const QString &groupName);//设置接收到群信息

private:
    void setInformation(const QPixmap &image, const QString &name, const QString &sign);//设置好友或群信息
    void tips(const QString &msg);//提示信息
    void searchFriend();//转换找好友功能
    void searchGroup();//转换找群功能
    void showAll();
    void hideAll();
    void imageProcessing(QPixmap &result, const QPixmap &image);

private slots:
    void on_lineEdit_returnPressed();//行编辑框回车
    void on_pushButton_clicked();//GO搜素
    void on_pushButton_2_clicked();//加好友

private:
    Ui::Search *ui;
    QPoint offSet;//鼠标和窗口位置偏移量

    bool which;//false查找好友,true查找群
    qulonglong inputId;//输入的好友或群id
    qulonglong recId;//接收的id

signals:
    void updateSearch(int type, qulonglong id);
};

#endif // SEARCH_H
