#-------------------------------------------------
#
# Project created by QtCreator 2017-07-05T14:21:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtLing
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    CompareFunc.cpp \
    Parse.cpp \
    StringFunc.cpp \
    StringSurrogate.cpp \
    Word.cpp \
    WordCollection.cpp

HEADERS  += mainwindow.h \
    CompareFunc.h \
    generaldefinitions.h \
    log2.h \
    Parse.h \
    StringFunc.h \
    StringSurrogate.h \
    Typedefs.h \
    Word.h \
    WordCollection.h

FORMS    += mainwindow.ui

DISTFILES += \
    test.txt \
    browncorpus.dx1
