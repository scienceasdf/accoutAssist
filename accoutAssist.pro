#-------------------------------------------------
#
# Project created by QtCreator 2017-12-01T23:45:56
#
#-------------------------------------------------

QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = accountAssist
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        acountassit.cpp \
    insertwidget.cpp \
    dailyreport.cpp \
    rangereport.cpp \
    customgesture.cpp \
    viewdetail.cpp \
    insertincome.cpp

HEADERS += \
        acountassit.h \
    connection.h \
    insertwidget.h \
    dailyreport.h \
    rangereport.h \
    customgesture.h \
    viewdetail.h \
    insertincome.h

FORMS += \
        acountassit.ui \
    insertwidget.ui \
    dailyreport.ui \
    rangereport.ui \
    viewdetail.ui \
    insertincome.ui

CONFIG += mobility
MOBILITY = 

win32:{
    DEFINES += FOR_DESKTOP
}
linux:{
    DEFINES += FOR_MOBILE
}


INCLUDEPATH += D:\qt-everywhere-opensource-src-5.9.1\qt-everywhere-opensource-src-5.9.1\qtbase\include\QtWidgets\5.9.1

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

RESOURCES += \
    skin.qrc

