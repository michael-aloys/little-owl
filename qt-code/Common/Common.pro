QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Common
TEMPLATE = lib

CONFIG += c++11 \
          staticlib

SOURCES += filedownloader.cpp \
    filereference.cpp \
    statefiles.cpp \
    hasher.cpp \
    filewriter.cpp \

HEADERS  += \
    filedownloader.h \
    filereference.h \
    statefiles.h \
    hasher.h \
    filewriter.h \

OTHER_FILES +=
