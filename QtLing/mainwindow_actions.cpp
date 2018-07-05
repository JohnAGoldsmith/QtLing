#include "mainwindow.h"
#include <QApplication>
#include <QAction>
#include <QToolBar>
#include <QDebug>
#include "mainwindow_menubar.h"

void MainWindow::createActions()
{
    /* --- FILE --- */
    // Open
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon("../../../../QtLing/images/open.png"));
    openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::ask_for_filename);

    // SaveAs
    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
    saveAsAct = new QAction(saveAsIcon, tr("Save &As ..."), this);
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));

    // Exit
    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    exitAct = new QAction(exitIcon, tr("E&xit"), this);
    connect(exitAct, &QAction::triggered, this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));

    /* --- EDIT --- */
    // Cut
    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon("../../../../QtLing/images/cut.png"));
    cutAct = new QAction(cutIcon, tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));

    // Copy
    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon("../../../../QtLing/images/copy.png"));
    copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));

    // Paste
    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon("../../../../QtLing/images/paste.png"));
    pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));

    // Find
    findAct = new QAction(tr("&Find in both tables"), this);
    connect(findAct, &QAction::triggered, this, &MainWindow::launch_find_dock);
    findAct->setShortcuts(QKeySequence::Find);
    findAct->setStatusTip(tr("Search for a string in the two upper tables"));

    findLeftAct = new QAction(tr("Find in upper-&Left table"), this);
    connect(findLeftAct, &QAction::triggered, this, &MainWindow::launch_find_dock);
    findAct->setStatusTip(tr("Search for a string in the upper-left table"));

    findRightAct = new QAction(tr("Find in upper-&Right table"), this);
    connect(findRightAct, &QAction::triggered, this, &MainWindow::launch_find_dock);
    findAct->setStatusTip(tr("Search for a string in the upper-right table"));


    /* --- HELP --- */
    // About
    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    // About Qt
    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));

    /* --- EVALUATE --- */
    // Import Gold Standard
    importAct = new QAction(tr("Import &Gold Standard XML file"), this);
    connect(importAct, &QAction::triggered, this, &MainWindow::gs_read_and_parse_xml);
    importAct->setStatusTip(tr("Import and Load Gold Standard XML file"));

    // Evaluate Gold Standard XML
    evalAct = new QAction(tr("&Evaluate current lexicon using Gold Standard"), this);
    connect(evalAct, &QAction::triggered, this, &MainWindow::gs_evaluate);
    evalAct->setStatusTip(tr("Evaluate output of linguistica using Gold Standard"));
    evalAct->setDisabled(true);

    // Import Morfessor XML
    importMorfessorAct = new QAction(tr("Import &Morfessor parses"), this);
    connect(importMorfessorAct, &QAction::triggered, this, &MainWindow::read_morfessor_txt_file);
    importMorfessorAct->setStatusTip(tr("Import Morfessor output file of word parses in txt format"));

    // Evaluate Morfessor XML
    evalMorfessorAct = new QAction(tr("Evaluate Morfessor parses"), this);
    connect(evalMorfessorAct, &QAction::triggered, this, &MainWindow::gs_evaluate_morfessor);
    evalMorfessorAct->setStatusTip(tr("Evaluate imported parses using gold standard"));
    evalMorfessorAct->setDisabled(true);

    // Create Menu Bar
    m_main_menu_bar = new MainMenuBar(this);
    setMenuBar(m_main_menu_bar);

    // Create Tool Bar
    QToolBar* fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);
    QToolBar* editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);


}
