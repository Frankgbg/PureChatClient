#include "chatfriend.h"
#include "homepage.h"
#include "information.h"
#include "login.h"
#include "mywidget.h"
#include "owninformation.h"
#include "search.h"

#include <QApplication>
#include <QFile>
#include <QJsonObject>
#include <QPixmap>
#include <QJsonDocument>
#include <QString>
#include <QSharedPointer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QHash<qulonglong, QSharedPointer<Search>> qmap;
//    qmap.insert(123456,QSharedPointer<Search>(new Search));
//    qDebug()<<(qmap.find(123456)==qmap.end());
//    qDebug()<<(qmap.find(12345)==qmap.end());
//    qmap.clear();
//    return 0;

    Login w;
    w.show();
    return a.exec();
}
