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
    string_group.cpp \
    cparse.cpp \
    lexicon_json.cpp \
    evaluation.cpp \
    evaluation_goldstandard.cpp \
    evaluation_evalparses.cpp \
    evaluation_parsemap.cpp \
    mainwindow_find.cpp \
    mainwindow_menubar.cpp \
    mainwindow_actions.cpp \
    stringalignment.cpp \
    allosignatures.cpp \
    compound.cpp \
    compoundcollection.cpp

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
    hypothesis.h \
    lxamodels.h \
    table_views.h\
    mainwindow.h \
    string_group.h \
    cparse.h \
    evaluation.h \
    mainwindow_find.h \
    mainwindow_menubar.h \
    stringalignment.h \
    allosignatures.h \
    compound.h \
    compoundcollection.h

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
