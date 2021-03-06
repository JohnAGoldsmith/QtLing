######################################################################
# Automatically generated by qmake (3.1) Sun Jul 11 16:15:04 2021
######################################################################

TEMPLATE = app
TARGET = QtLing
INCLUDEPATH += .

QT += gui widgets
QT += xml

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += allosignatures.h \
           CompareFunc.h \
           compound.h \
           cparse.h \
           evaluation.h \
           generaldefinitions.h \
           graphics.h \
           hypothesis.h \
           Lexicon.h \
           log2.h \
           lxamodels.h \
           mainwindow.h \
           mainwindow_find.h \
           mainwindow_menu_copy.h \
           mainwindow_menubar.h \
           parsings.h \
           Signature.h \
           SignatureCollection.h \
           Stem.h \
           StemCollection.h \
           string_group.h \
           stringalignment.h \
           Suffix.h \
           SuffixCollection.h \
           supersignature.h \
           table_views.h \
           Typedefs.h \
           ui_mainwindow.h \
           Word.h \
           WordCollection.h
SOURCES += allosignatures.cpp \
           CompareFunc.cpp \
           compound.cpp \
           corpus.cpp \
           cparse.cpp \
           display_collections.cpp \
           evaluation.cpp \
           evaluation_evalparses.cpp \
           evaluation_goldstandard.cpp \
           evaluation_parsemap.cpp \
           graphics.cpp \
           graphics_hypotheses.cpp \
           graphics_polygons.cpp \
           hypothesis.cpp \
           Lexicon.cpp \
           lexicon_crab1.cpp \
           lexicon_crab2.cpp \
           lexicon_json.cpp \
           lexicon_sublexicon.cpp \
           lxamodels.cpp \
           main.cpp \
           mainwindow.cpp \
           mainwindow_actions.cpp \
           mainwindow_find.cpp \
           mainwindow_menubar.cpp \
           mainwindow_treeview.cpp \
           output.cpp \
           parsings.cpp \
           Signature.cpp \
           signature_graph.cpp \
           SignatureCollection.cpp \
           Stem.cpp \
           StemCollection.cpp \
           string_group.cpp \
           stringalignment.cpp \
           Suffix.cpp \
           SuffixCollection.cpp \
           supersignature.cpp \
           table_display_functions.cpp \
           table_views_lower.cpp \
           table_views_upper.cpp \
           Word.cpp \
           WordCollection.cpp
