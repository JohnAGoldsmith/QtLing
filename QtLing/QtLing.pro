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
    Word.cpp \
    WordCollection.cpp \
    Lexicon.cpp \
    SignatureCollection.cpp \
    Signature.cpp \
    StemCollection.cpp \
    Stem.cpp \
    SuffixCollection.cpp \
    Suffix.cpp \
    graphics.cpp \
    lxamodels.cpp \
    supersignature.cpp \
    display_collections.cpp \
    hypothesis.cpp \
    graphics_polygons.cpp \
    graphics_hypotheses.cpp \
    table_display_functions.cpp \
    table_views_lower.cpp \
    table_views_upper.cpp \
    output.cpp \
    corpus.cpp \
    lexicon_sublexicon.cpp \
    lexicon_crab2.cpp \
    signature_graph.cpp \
    parsings.cpp

HEADERS  += mainwindow.h \
    CompareFunc.h \
    generaldefinitions.h \
    log2.h \
    Typedefs.h \
    Word.h \
    WordCollection.h \
    Lexicon.h \
    SignatureCollection.h \
    Signature.h \
    StemCollection.h \
    Stem.h \
    SuffixCollection.h \
    Suffix.h \
    graphics.h \
    ui_mainwindow.h \
    supersignature.h \
    hypothesis.h \
    lxamodels.h \
    table_views.h\
    mainwindow.h \
    parsings.h

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
