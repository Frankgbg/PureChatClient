#ifndef CREATGROUP_H
#define CREATGROUP_H

#include <QWidget>

namespace Ui {
class CreateGroup;
}

class CreateGroup : public QWidget
{
    Q_OBJECT
public:
    explicit CreateGroup(QWidget *parent = nullptr);
    ~CreateGroup();

    void mousePressEvent(QMouseEvent *event) override;//鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;//鼠标移动事件
    bool eventFilter(QObject *object, QEvent *e) override;//事件过滤器

private slots:
    void on_pushButton_clicked();

private:
    Ui::CreateGroup *ui;
    QPoint offSet;//鼠标和窗口位置偏移量

signals:
    void createGroup(QString groupName);
};

#endif // CREATGROUP_H
