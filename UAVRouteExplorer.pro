QT       += core gui

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
    AStar.cpp \
    aboutdialog.cpp \
    dashboard.cpp \
    imageview.cpp \
    main.cpp \
    mainwindow.cpp \
    uavrouteexplorer.cpp \
    waintingdialog.cpp

HEADERS += \
    AStar.hpp \
    aboutdialog.h \
    dashboard.h \
    imageview.h \
    mainwindow.h \
    uavrouteexplorer.h \
    waintingdialog.h

FORMS += \
    aboutdialog.ui \
    dashboard.ui \
    mainwindow.ui \
    waintingdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc

RC_ICONS = UAV.ico

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Users/wenbo.hu/AppData/Local/Programs/Python/Python39/libs/ -lpython39
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Users/wenbo.hu/AppData/Local/Programs/Python/Python39/libs/ -lpython39_d
else:unix: LIBS += -L$$PWD/../../Users/wenbo.hu/AppData/Local/Programs/Python/Python39/libs/ -lpython39

INCLUDEPATH += $$PWD/../../Users/wenbo.hu/AppData/Local/Programs/Python/Python39/include
DEPENDPATH += $$PWD/../../Users/wenbo.hu/AppData/Local/Programs/Python/Python39/include
