QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Parent
TEMPLATE = app

CONFIG += c++11

SOURCES += \
    parentowl.cpp \ 
    main.cpp

HEADERS  += \
    parentowl.h 
    
INCLUDEPATH += ../Common
LIBS += -L../Common -lCommon

OTHER_FILES +=
