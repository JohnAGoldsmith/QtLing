#-------------------------------------------------
#
# Project created by QtCreator 2017-07-05T14:21:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtLing
TEMPLATE = app


SOURCES += \ 
    CompareFunc.cpp \
    display_collections.cpp \
    graphics.cpp \
    graphics_polygons.cpp \
    hypothesis.cpp \
    Lexicon.cpp \
    lxamodels.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindow_display_functions.cpp \
    Signature.cpp \
    SignatureCollection.cpp \
    Stem.cpp \
    StemCollection.cpp \
    Suffix.cpp \
    SuffixCollection.cpp \
    supersignature.cpp \
    table_views.cpp \
    Word.cpp \
    WordCollection.cpp

HEADERS  += \ 
    CompareFunc.h \
    generaldefinitions.h \
    graphics.h \
    hypothesis.h \
    Lexicon.h \
    log2.h \
    lxamodels.h \
    mainwindow.h \
    Signature.h \
    SignatureCollection.h \
    Stem.h \
    StemCollection.h \
    Suffix.h \
    SuffixCollection.h \
    supersignature.h \
    table_views.h \
    Typedefs.h \
    ui_mainwindow.h \
    Word.h \
    WordCollection.h

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
