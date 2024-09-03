QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Login/logindialog.cpp \
    Seller/catalogdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    mysql.cpp \
    Login/regdialog.cpp \
    Seller/sellerdialog.cpp \
    productdialog.cpp \
    reviewdialog.cpp

HEADERS += \
    Login/logindialog.h \
    Seller/catalogdialog.h \
    mainwindow.h \
    mysql.h \
    Login/regdialog.h \
    Seller/sellerdialog.h \
    productdialog.h \
    reviewdialog.h

FORMS += \
    Login/logindialog.ui \
    Seller/catalogdialog.ui \
    mainwindow.ui \
    Login/regdialog.ui \
    Seller/sellerdialog.ui \
    productdialog.ui \
    reviewdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
