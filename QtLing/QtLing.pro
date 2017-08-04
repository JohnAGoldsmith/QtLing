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
    WordCollection.cpp \
    Lexicon.cpp \
    SignatureCollection.cpp \
    Signature.cpp \
    StemCollection.cpp \
    Stem.cpp \
    SuffixCollection.cpp \
    Suffix.cpp

HEADERS  += mainwindow.h \
    CompareFunc.h \
    generaldefinitions.h \
    log2.h \
    Parse.h \
    StringFunc.h \
    StringSurrogate.h \
    Typedefs.h \
    Word.h \
    WordCollection.h \
    Lexicon.h \
    SignatureCollection.h \
    Signature.h \
    StemCollection.h \
    Stem.h \
    SuffixCollection.h \
    Suffix.h

FORMS    +=

DISTFILES += \
    test.txt \
    browncorpus.dx1 \
    images/copy.png \
    images/cut.png \
    images/new.png \
    images/open.png \
    images/paste.png \
    images/save.png
