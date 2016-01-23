# Child (Client) code

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Child
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
    childowl.cpp \
    childwindow.cpp

HEADERS  += \
    childowl.h \
    childwindow.h

FORMS    += childwindow.ui

INCLUDEPATH += ../Common
LIBS += -L../Common -lCommon

OTHER_FILES +=
