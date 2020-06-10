#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    MyWidget(int width, int height, QWidget *parent = nullptr);
    ~MyWidget();

public:
    void init(qulonglong id, const QPixmap &pixmap,
              const QString &name, const QString &sign = "");

    void setHasSign(const bool &hasSign);//设置是否有签名
    bool getHasSign() const { return _hasSign;}//返回是否有签名

    void setPixmap(const QPixmap &pixmap);//设置头像
    QPixmap getPixmap() const;//返回头像

    qulonglong getId() const;//返回Id

    void setName(const QString &name);//设置昵称
    QString getName() const;//返回昵称

    void setSign(const QString &sign);//设置签名
    QString getSign() const;//返回签名

private:
    void change();//改变有无签名
    void changeNameLabel(double factor);//改变昵称标签
    void imageProcessing(QPixmap &result, const QPixmap &pixmap);

private:
    Ui::MyWidget *ui;
    int _width;
    int _height;
    bool _hasSign;
    qulonglong _id;
    QPixmap _pixmap;
};

#endif // MYWIDGET_H
