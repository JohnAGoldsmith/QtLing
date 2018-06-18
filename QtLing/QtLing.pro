#-------------------------------------------------
#
# Project created by QtCreator 2017-07-05T14:21:25
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtLing
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    corpus.cpp \
    CompareFunc.cpp \
    lexicon_sublexicon.cpp \
    lexicon_crab2.cpp \
    Word.cpp \
    WordCollection.cpp \
    Lexicon.cpp \
    output.cpp \
    SignatureCollection.cpp \
    signature_graph.cpp\
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
    goldstandard.cpp \
    mainmenu.cpp

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
    table_views.h \
    goldstandard.h \
    mainmenu.h

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
