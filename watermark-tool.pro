QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about.cpp \
    fimagefileinfo.cpp \
    imagepreviewitem.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    about.h \
    fimagefileinfo.h \
    imagepreviewitem.h \
    mainwindow.h

FORMS += \
    about.ui \
    mainwindow.ui

TRANSLATIONS += \
    watermark-tool_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

target_icons.files += $$PWD/icons/*
target_icons.path=/usr/share/watermark-tool/icons/
!isEmpty(target_icons.path): INSTALLS += target_icons

target_desktop.files += $$PWD/watermark-tool.desktop
target_desktop.path=/usr/share/applications/
!isEmpty(target_desktop.path): INSTALLS += target_desktop
