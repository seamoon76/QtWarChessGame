QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += resources_big

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    isoldier.cpp \
    itemforeffect.cpp \
    itemforrole.cpp \
    main.cpp \
    mainwindow.cpp \
    mapinfo.cpp \
    monster.cpp \
    myitem.cpp \
    mymessagebox.cpp \
    mytip.cpp \
    playwindow.cpp \
    selectlevelwindow.cpp

HEADERS += \
    isoldier.h \
    itemforeffect.h \
    itemforrole.h \
    mainwindow.h \
    mapinfo.h \
    monster.h \
    myconstant.h \
    myitem.h \
    mymessagebox.h \
    mytip.h \
    playwindow.h \
    selectlevelwindow.h

FORMS += \
    mainwindow.ui \
    mytip.ui \
    playwindow.ui \
    selectlevelwindow.ui

TRANSLATIONS += \
    MonsterInvasion_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

DISTFILES += \
    icon.rc
