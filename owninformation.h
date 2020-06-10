#ifndef OWNINFORMATION_H
#define OWNINFORMATION_H

#include <QWidget>

namespace Ui {
class OwnInformation;
}

class OwnInformation : public QWidget
{
    Q_OBJECT
public:
    explicit OwnInformation(qulonglong ownId, const QPixmap &ownImage, const QString &ownName, const QString &ownSign,
                            const QString &ownPassword, QWidget *parent = nullptr);
    ~OwnInformation();

    void mousePressEvent(QMouseEvent *event) override;//鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;//鼠标移动事件
    bool eventFilter(QObject *object, QEvent *e) override;//事件过滤器

public:
    void receive(int type, const QString &msg);

private slots:
    void on_lineEdit_returnPressed();//昵称
    void on_lineEdit_2_returnPressed();//签名
    void on_pushButton_clicked();//上传头像；暂未开发
    void on_pushButton_2_clicked();//修改密码
    void on_pushButton_3_clicked();//提交密码

private:
    void change();//改变信息和修改密码界面
    void toUpdatePassword();//修改界面
    void returnInformation();//返回信息界面
    void tips(const QString &msg);
    void imageProcessing(QPixmap &result, const QPixmap &pixmap);

private:
    Ui::OwnInformation *ui;
    QPoint offSet;//鼠标和窗口位置偏移量
    bool which;
    qulonglong _ownId;
    QPixmap _ownImage;
    QString _ownName;
    QString _ownSign;
    QString _ownPassword;

signals:
    void updateInformation(int type, qulonglong id, const QString &information);//更改信息
    void updateInformationSuccessful(int type, qulonglong id, const QString &information);//更改信息成功
    void updateImage(int type, qulonglong id, const QPixmap &newImage, const QString &imageType);//更改头像
    void updateImageSuccessful(int type, qulonglong id, const QPixmap &newImage);//更改头像成功
};

#endif // OWNINFORMATION_H
