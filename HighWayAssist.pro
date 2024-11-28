QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport multimediawidgets multimedia sql

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    general.cpp \
    lanemarkers.cpp \
    lanemkr.cpp \
    main.cpp \
    mainwindow.cpp \
    mysqlite.cpp \
    pahhist.cpp \
    qcustomplot.cpp \
    roadedge.cpp \
    roadsign.cpp \
    sfobject.cpp \
    vehself.cpp

HEADERS += \
    CustomDataType.h \
    general.h \
    lanemarkers.h \
    lanemkr.h \
    mainwindow.h \
    mysqlite.h \
    pahhist.h \
    qcustomplot.h \
    roadedge.h \
    roadsign.h \
    sfobject.h \
    vehself.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
