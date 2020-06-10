QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chatfriend.cpp \
    chatgroup.cpp \
    creategroup.cpp \
    emojs.cpp \
    friendlist.cpp \
    grouplist.cpp \
    homepage.cpp \
    information.cpp \
    main.cpp \
    login.cpp \
    mylistwidget.cpp \
    mywidget.cpp \
    owninformation.cpp \
    search.cpp \
    socket.cpp \
    socketthread.cpp

HEADERS += \
    chatfriend.h \
    chatgroup.h \
    creategroup.h \
    emojs.h \
    friendlist.h \
    grouplist.h \
    homepage.h \
    information.h \
    login.h \
    mylistwidget.h \
    mywidget.h \
    owninformation.h \
    search.h \
    socket.h \
    socketthread.h

FORMS += \
    chatfriend.ui \
    chatgroup.ui \
    creategroup.ui \
    emojs.ui \
    homepage.ui \
    information.ui \
    login.ui \
    mywidget.ui \
    owninformation.ui \
    search.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images/images.qrc \
    qss/qss.qrc
