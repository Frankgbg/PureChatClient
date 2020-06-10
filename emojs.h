#ifndef EMOJS_H
#define EMOJS_H

#include <QWidget>

namespace Ui {
class Emojs;
}

class Emojs : public QWidget
{
    Q_OBJECT
public:
    explicit Emojs(QWidget *parent = nullptr);
    ~Emojs();

    bool eventFilter(QObject *object, QEvent *e) override;

private:
    Ui::Emojs *ui;

signals:
     void imgUrl(QString imgUrl);
};

#endif // EMOJS_H
