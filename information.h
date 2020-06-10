#ifndef INFORMATION_H
#define INFORMATION_H

#include <QWidget>

namespace Ui {
class Information;
}

class Information : public QWidget
{
    Q_OBJECT
public:
    explicit Information(qulonglong id, const QPixmap &image, const QString &name,
                         const QString &sign, bool editabled = false, QWidget *parent = nullptr);
    ~Information();

    void mousePressEvent(QMouseEvent *event) override;//鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;//鼠标移动事件
    bool eventFilter(QObject *object, QEvent *e) override;//事件过滤器

public:
    inline qulonglong getId() const { return _id;}
    void receive(int type, const QString &msg);

private slots:
    void on_lineEdit_returnPressed();
    void on_pushButton_clicked();

private:
    void tips(const QString &msg);
    void imageProcessing(QPixmap &result, const QPixmap &pixmap);

private:
    Ui::Information *ui;
    QPoint offSet;//鼠标和窗口位置偏移量
    qulonglong _id;
    QPixmap _image;
    QString _name;
    bool _editabled;

signals:
    void updateInformation(int type, qulonglong id, const QString &information);//更改群信息
    void updateInformationSuccessful(int type, qulonglong id, const QString &information);//更改群信息成功
    void updateImage(int type, qulonglong id, const QPixmap &newImage, const QString &imageType);//更改群头像
    void updateImageSuccessful(int type, qulonglong id, const QPixmap &newImage);//更改群头像成功
    void removeThis(bool _editabled, qulonglong id);
};

#endif // INFORMATION_H
