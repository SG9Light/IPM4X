QT       += core gui axcontainer serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    acsoucedialog.cpp \
    comboxex.cpp \
    datamonitoring.cpp \
    main.cpp \
    mainwindow.cpp \
    modbusslave.cpp \
    sqldialog.cpp \
    threadpool.cpp \
    types.cpp \
    userport.cpp

HEADERS += \
    acsoucedialog.h \
    comboxex.h \
    consts.h \
    datamonitoring.h \
    mainwindow.h \
    modbusslave.h \
    sqldialog.h \
    threadpool.h \
    types.h \
    userport.h

FORMS += \
    acsoucedialog.ui \
    datamonitoring.ui \
    mainwindow.ui \
    sqldialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = logo.ico

DISTFILES += \
    icon/refresh.png

RESOURCES += \
    logo.qrc

