#include "mainwindow.h"
//#include "generaldefinitions.h"
#include "ui_mainwindow.h"
#include <QBrush>
#include <QColor>
#include <QtWidgets>
#include <QTreeView>
#include <QKeyEvent>
#include "iostream"
#include <QStandardItemModel>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <algorithm>
#include <stdio.h>
#include "WordCollection.h"
#include "Word.h"
#include "StemCollection.h"
#include "Stem.h"
#include "SuffixCollection.h"
#include "Suffix.h"
#include "SignatureCollection.h"
#include "Signature.h"
#include <QKeyEvent>
#include <QtWidgets>
#include <QString>
#include <QDebug>
#include <QPair>
#include <QMapIterator>
#include <QDir>

#include <QSplitter>
#include <QTableView>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSortFilterProxyModel>
#include "mainwindow.h"
#include "graphics.h"
#include "generaldefinitions.h"
#include "lxamodels.h"

class LxaStandardItemModel;


MainWindow::MainWindow()
{

    m_lexicon_list.append ( new CLexicon() );
    CLexicon * lexicon =  m_lexicon_list.last();

    // models
    m_Models["Words"]                       = new LxaStandardItemModel("Words");
    m_Models["Suffixal stems"]              = new LxaStandardItemModel("Suffixal stems");
    m_Models["Prefixal stems"]              = new LxaStandardItemModel("Prefixal stems");
    m_Models["Suffixes"]                    = new LxaStandardItemModel("Suffixes");
    m_Models["Signatures"]                  = new LxaStandardItemModel("Signatures");
    m_Models["Signatures 2"]                = new LxaStandardItemModel("Signatures");// sorted by affix count;
    m_Models["Signatures 3"]                = new LxaStandardItemModel("Signatures");// used temporarily;
    m_Models["EPositive signatures"]        = new LxaStandardItemModel("EPositive signatures");
    m_Models["EPositive signatures 2"]      = new LxaStandardItemModel("EPositive signatures 2"); // sorted by affix count
    m_Models["Prefix signatures"]           = new LxaStandardItemModel("Prefix signatures");
    m_Models["Prefix signatures 2"]         = new LxaStandardItemModel("Prefix signatures"); //sorted by affix count;
    m_Models["EPositive prefix signatures"] = new LxaStandardItemModel("EPositive prefix signatures");
    m_Models["EPositive prefix signatures 2"]= new LxaStandardItemModel("EPositive prefix signatures");
    m_Models["Residual parasignatures"]     = new LxaStandardItemModel("Residual parasignatures");
    m_Models["SigGraphEdges_1"]               = new LxaStandardItemModel("SigTreeEdges_1");
    m_Models["SigGraphEdges_2"]               = new LxaStandardItemModel("SigTreeEdges_2");
    m_Models["Parasuffixes"]                = new LxaStandardItemModel("Parasuffixes");
    m_Models["Passive signatures"]          = new LxaStandardItemModel("Passive signatures");
    m_Models["Hypotheses"]                  = new LxaStandardItemModel("Hypotheses");
    m_Models["Hypotheses 2"]                = new LxaStandardItemModel("Hypotheses 2");
// add component 3


//  this is part of an experiment:
    QMap<QString,LxaStandardItemModel*> temp_Models;// this will become "m_Models";
    QMapIterator<QString,eComponentType> iter (m_lexicon_list.last()->get_category_types());
    while (iter.hasNext()){
        QString key = iter.next().key();
        temp_Models[key] = new LxaStandardItemModel(key);
    }
//  end of experiment


    m_treeModel     = new QStandardItemModel();

    get_lexicon()->set_status_bar(statusBar());

    // views
    m_leftTreeView              = new LeftSideTreeView(this);
    m_tableView_upper_left      = new UpperTableView (this);
    m_tableView_upper_right     = new UpperTableView (this,  SIG_BY_AFFIX_COUNT);
    m_tableView_lower           = new LowerTableView (this);
    m_tableView_upper_left->setSortingEnabled(true);
    m_tableView_upper_right->setSortingEnabled(true);

//    bool suffix_flag = true;

    m_graphics_scene            = new lxa_graphics_scene( this, lexicon);
    m_graphics_view             = new lxa_graphics_view(m_graphics_scene, this);
    m_graphic_display_flag      = false;             // toggle with Ctrl-G
    set_up_graphics_scene_and_view();

    //<--------------     set up main window widget ------------------------->
    // set model for tree view
    m_leftTreeView->setModel(m_treeModel);
    // whole window
    m_mainSplitter = new QSplitter();
    // on right side:
    m_rightSplitter = new QSplitter(Qt::Vertical);

    // new stuff:
    m_top_rightSplitter = new QSplitter(Qt::Horizontal);
    m_top_rightSplitter->addWidget(m_tableView_upper_left);
    m_top_rightSplitter->addWidget(m_tableView_upper_right );

    // entire right side:
    m_rightSplitter->addWidget(m_top_rightSplitter);
    m_rightSplitter->addWidget(m_tableView_lower);

    // top level (whole window)
    // on left side:
    m_mainSplitter->addWidget(m_leftTreeView);
    m_mainSplitter->addWidget(m_rightSplitter);

    setCentralWidget(m_mainSplitter);

    m_ProgressBar  = new QProgressBar(this);
    m_ProgressBar->setVisible(true);
    statusBar()->addPermanentWidget(m_ProgressBar);
    get_lexicon()->set_progress_bar (m_ProgressBar);

    createActions();
    createStatusBar();
    readSettings();


    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    m_mainSplitter->setSizes(QList<int>() << 1000 <<4000);

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);

    connect(m_leftTreeView, SIGNAL(clicked(const QModelIndex&)),
            m_tableView_upper_left, SLOT(ShowModelsUpperTableView(const QModelIndex&)));
    connect(m_leftTreeView, SIGNAL(clicked(const QModelIndex&)),
            m_tableView_upper_right, SLOT(ShowModelsUpperTableView(const QModelIndex&)));


    // clicking on the upperleft corner can signal a graphic view below it, or a table below it.
    connect(m_tableView_upper_left,SIGNAL(clicked(const QModelIndex & )),
            m_tableView_lower,SLOT(display_this_item(const QModelIndex &  )));
//    connect(m_tableView_upper_left,SIGNAL(clicked(const QModelIndex & )),
//            m_current_graphics_scene,SLOT(display_this_item(const QModelIndex &  )));



    connect(m_tableView_upper_right,SIGNAL(clicked(const QModelIndex & )),
            m_tableView_lower,SLOT(display_this_item(const QModelIndex &  )));

    connect(m_tableView_upper_left,SIGNAL(clicked(const QModelIndex & )),
            m_tableView_upper_right,SLOT(display_this_affixes_signatures(const QModelIndex &  )));



}

void MainWindow::keyPressEvent(QKeyEvent* ke)
{
    int this_key = ke->key();

    switch(this_key){
    case Qt::Key_1:
    {
        if (get_lexicon()->get_prefixal_stems()->get_count() > 0){
            get_lexicon()->set_prefixes_flag();
            do_crab2();
            display_prefix_signatures(get_lexicon());
        }
        break;
     }
    case Qt::Key_2:
    {
        if (get_lexicon()->get_suffixal_stems()->get_count() > 0){
           get_lexicon()->set_suffixes_flag();
           do_crab2();
           //display_suffix_signatures();
        } else{
            get_lexicon()->set_prefixes_flag();
            do_crab2();
            display_prefix_signatures(get_lexicon());
        }
        break;
    }
    case  Qt::Key_3:    {
        statusBar()->showMessage(tr("Read file."), 5000);
        ask_for_project_file();
        break;
    }
    case Qt::Key_4:{
        read_corpus();
        analyze_corpus();
        break;
    }
    case Qt::Key_5:{
        CLexicon* sublexicon = get_lexicon()->build_sublexicon();
        load_models(sublexicon);
        if (sublexicon->get_suffix_flag()){
            display_suffix_signatures(sublexicon);
        } else{
            display_prefix_signatures(sublexicon);
        }
        break;
    }
    case Qt::Key_6:{
        get_lexicon()->set_prefixes_flag();
        break;
    }
    case Qt::Key_D:     {
        read_dx1_file();
        break;
    }
    case Qt::Key_G:
    {
        if (m_graphic_display_flag==false){
            m_rightSplitter->replaceWidget(1,m_graphics_view);
            m_graphics_view->centerOn(0,1000);// this should be fixed so that the initial showing of the graphic is done right.
            m_graphic_display_flag = true;
            m_rightSplitter->setFocus();
            QList<int> sizes;
            sizes<< 200 << 500;
            m_rightSplitter->setSizes(sizes);
        } else{
            m_rightSplitter->replaceWidget(1,m_tableView_lower);
            m_graphic_display_flag = false;
            QList<int> sizes;
            sizes<< 500 << 200;
            m_rightSplitter->setSizes(sizes);
        }
        break;
    }
    case Qt::Key_I: {
        m_graphics_scene->narrow_columns();
            break;
    }
    case Qt::Key_J: {
        m_graphics_scene->move_rows_apart();
            break;
    }
    case Qt::Key_K: {
        m_graphics_scene->move_rows_closer();
        break;
    }
    case Qt::Key_L:    {
        m_graphics_view->zoom_up();
        break;
    }
    case Qt::Key_S:
    {
        get_lexicon()->set_suffixes_flag();
        do_crab();
        display_suffix_signatures(get_lexicon());
        break;
    }
    case Qt::Key_V:
    {
    }
    case Qt::Key_U:
    {
        m_graphics_scene->widen_columns();
        break;
    }
    case Qt::Key_P:
    {
        get_lexicon()->set_prefixes_flag();
        do_crab();
        display_prefix_signatures(get_lexicon());
        break;
    }
    case Qt::Key_Q:
    {
         break;
    }
    case Qt::Key_Semicolon:
    {
        m_graphics_view->zoom_down();
        break;
    }
    case Qt::Key_Exclam:
    {
        m_graphics_view->reset_scale();
        break;
    }
    case Qt::Key_H:
    {
        QMainWindow::keyPressEvent(ke);
        break;
    }
    }
}



void MainWindow::do_crab()
{   statusBar()->showMessage("Entering the Crab Nebula.");
    get_lexicon()->Crab_1();
    load_models(get_lexicon());
    write_stems_and_words();

    statusBar()->showMessage("We have returned from the Crab Nebula.");
    create_or_update_TreeModel(get_lexicon());

    set_up_graphics_scene_and_view();
    m_leftTreeView->expandAll();
    m_leftTreeView->resizeColumnToContents(0);
    statusBar()->showMessage("All models are loaded.");
}

void MainWindow::newFile()
{
    if (ask_to_save()) {
        setCurrentFile(QString());
    }
}

void MainWindow::ask_for_filename()
{
    qDebug() << " ask for filename" ;
    m_name_of_data_file = QFileDialog::getOpenFileName(this);
    qDebug() << m_name_of_data_file;
    read_dx1_file();
}

void MainWindow::ask_for_project_file()
{
    m_name_of_project_file = QFileDialog::getOpenFileName (this);
    read_stems_and_words();

    //display_suffix_signatures(get_lexicon());
}


void MainWindow::load_models(CLexicon* lexicon)
{

    m_Models["Words"]               ->load_words(lexicon->get_words());
    statusBar()->showMessage("Words.");
    QCoreApplication::processEvents();

    m_Models["Suffixal stems"]      ->load_stems(lexicon->get_suffixal_stems());
    statusBar()->showMessage("Suffixal stems.");
    QCoreApplication::processEvents();

    m_Models["Prefixal stems"]      ->load_stems(lexicon->get_prefixal_stems());
    statusBar()->showMessage("Prefixal stems.");
    QCoreApplication::processEvents();

    m_Models["Suffixes"]            ->load_suffixes(lexicon->get_suffixes());
    statusBar()->showMessage("Suffixes.");
    QCoreApplication::processEvents();

    m_Models["Signatures"]          ->load_signatures(lexicon->get_signatures());
    statusBar()->showMessage("Signatures.");
    QCoreApplication::processEvents();

    m_Models["Signatures 2"]         ->load_signatures(lexicon->get_signatures(), SIG_BY_AFFIX_COUNT);
    statusBar()->showMessage("Signatures 2.");
    QCoreApplication::processEvents();

    m_Models["Signatures 3"]         ->load_signatures(lexicon->get_signatures());
    statusBar()->showMessage("Signatures 3.");
    QCoreApplication::processEvents();

    m_Models["EPositive signatures"]->load_positive_signatures(lexicon->get_signatures());
    statusBar()->showMessage("EPositive signatures.");

    m_Models["EPositive signatures 2"]->load_positive_signatures(lexicon->get_signatures(),SIG_BY_AFFIX_COUNT);
    statusBar()->showMessage("EPositive signatures 2.");


    QCoreApplication::processEvents();


    m_Models["Prefix signatures"]   ->load_signatures( lexicon->get_prefix_signatures());
    m_Models["Prefix signatures 2"] ->load_signatures(lexicon->get_prefix_signatures(), SIG_BY_AFFIX_COUNT);
    m_Models["EPositive prefix signatures"]->load_positive_signatures(lexicon->get_prefix_signatures());
    m_Models["EPositive prefix signatures 2"]->load_positive_signatures(lexicon->get_prefix_signatures(), SIG_BY_AFFIX_COUNT);

    m_Models["Residual parasignatures"]->load_parasignatures(lexicon->get_residual_signatures());
    m_Models["Parasuffixes"]        ->load_suffixes(lexicon->get_parasuffixes());
    m_Models["Passive signatures"]  ->load_signatures(lexicon->get_passive_signatures());
    m_Models["Hypotheses"]          ->load_hypotheses(lexicon->get_hypotheses());
    m_Models["Hypotheses 2"]        ->load_hypotheses_2(lexicon->get_hypotheses());
    m_Models["SigGraphEdges_1"]        ->load_sig_graph_edges(lexicon->get_sig_graph_edge_map(),1);
    m_Models["SigGraphEdges_2"]        ->load_sig_graph_edges(lexicon->get_sig_graph_edge_map(),2);


}
void MainWindow::set_up_graphics_scene_and_view()
{
    if (get_lexicon()->get_suffix_flag())
    {
          m_graphics_scene ->ingest(get_lexicon(),get_lexicon()->get_signatures(), true);
      }
    else
    {
          m_graphics_scene->ingest(get_lexicon(), get_lexicon()->get_prefix_signatures(), false);
    }
}

void MainWindow::do_crab2()
{
    statusBar()->showMessage("Entering the Crab Nebula, phase 2");
    CLexicon* lexicon = get_lexicon();
    lexicon->Crab_2();
    load_models(get_lexicon());



    create_or_update_TreeModel(get_lexicon());

// add component 5


    //     part of an experiment:
    //     QMapIterator<QString,eComponentType> iter (get_lexicon()->get_category_types());
    //     while (iter.hasNext()){
    //      QString component_name = iter.next().key();
    //      eComponentType this_component_type = iter.value();
    //      m_Models[component_name]->load_category(component_name, this_component_type);
    //    }
    // end of experiment


    print_prefix_signatures();
    m_graphics_scene->clear_all();
    m_leftTreeView->expandAll();
    m_leftTreeView->resizeColumnToContents(0);

    write_stems_and_words();

    statusBar()->showMessage("All models are loaded.");
}
void MainWindow::read_file_do_crab()
{       read_dx1_file();
        statusBar()->showMessage(tr("Ready"));
        do_crab();
}


void MainWindow::read_dx1_file()
{
     QFile file(m_name_of_data_file);
     if (!file.open(QFile::ReadOnly | QFile::Text))
     {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(m_name_of_data_file), file.errorString()));
            return;
     }

     QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
     settings.setValue("name_of_data_file", m_name_of_data_file );

     QTextStream in(&file);
     CWordCollection * Words = get_lexicon()->get_word_collection();

     while (!in.atEnd())
     {
            QString line = in.readLine();
            line = line.simplified(); // get rid of extra spaces
            QStringList words = line.split(" ");
            QString word = words[0];
            word = word.toLower();
            CWord* pWord = *Words <<  word;
            if (words.size()> 1) {
                pWord->SetWordCount(words[1].toInt());
            }
     }
    Words->sort_word_list();
    setCurrentFile(m_name_of_data_file);
    statusBar()->showMessage(tr("File loaded"), 5000);
}




bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}
bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}
void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("Linguistica 2017: Crab Nebula. "
                ));
}
void MainWindow::documentWasModified()
{

}






void MainWindow::print_prefix_signatures()
{
    CSignature* pSig;
    int count = 0;
    CStem *  pStem;

    QString filename = "signatures.txt";
    QFile file (filename);
    if (file.open(QIODevice::ReadWrite)){

        QTextStream stream( &file);


    map_sigstring_to_sig_ptr_iter sig_iter (*get_lexicon()->get_prefix_signatures()->get_map());
    while (sig_iter.hasNext()){
       pSig = sig_iter.next().value();
       stream << pSig->get_key()<< endl;
       CStem_ptr_list_iterator stem_iter (*pSig->get_stems());
       while (stem_iter.hasNext()){
           pStem = stem_iter.next();
           stream << pStem->get_key() << "\t";
           count++;
           if (count ==5){
               count = 0;
               stream << endl;
           }
       }
       stream << endl << endl;
       count = 0;
    }
    stream << endl;

    }
    file.close();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//        Infra- and super-structure
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::createActions()
{

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
//---------------------> --------------------------> -------------------------->
//    QAction * suffix_signature_display_action = new QAction();
//    CLexicon* lexicon = get_lexicon();
//    connect(suffix_signature_display_action, &QAction::triggered, this,  &MainWindow::display_suffix_signatures );

//    QAction * prefix_signature_display_action = new QAction();
//    connect(prefix_signature_display_action, &QAction::triggered, this,  &MainWindow::display_prefix_signatures );
//---------------------> --------------------------> -------------------------->


    // Give a data file name, store the name, and read the file.
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon("../../../../QtLing/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::ask_for_filename);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    // No action associated with this yet.
    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
    QAction *saveAsAct = fileMenu->addAction(saveAsIcon, tr("Save &As..."), this, &MainWindow::saveAs);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));

    fileMenu->addSeparator();

    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));

#ifndef QT_NO_CLIPBOARD
    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon("../../../../QtLing/images/cut.png"));
    QAction *cutAct = new QAction(cutIcon, tr("Cu&t"), this);

    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
//    connect(cutAct, &QAction::triggered, textEdit, &QPlainTextEdit::cut);
    editMenu->addAction(cutAct);
    editToolBar->addAction(cutAct);

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon("../../../../QtLing/images/copy.png"));
    QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
//    connect(copyAct, &QAction::triggered, textEdit, &QPlainTextEdit::copy);
    editMenu->addAction(copyAct);
    editToolBar->addAction(copyAct);

    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon("../../../../QtLing/images/paste.png"));
    QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
//    connect(pasteAct, &QAction::triggered, textEdit, &QPlainTextEdit::paste);
    editMenu->addAction(pasteAct);
    editToolBar->addAction(pasteAct);

    menuBar()->addSeparator();

#endif // !QT_NO_CLIPBOARD

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));


    //fileToolBar->addButton("Sort");
}
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}
void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    m_name_of_data_file = settings.value("name_of_data_file").toString();

    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}
void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("name_of_data_file", m_name_of_data_file );

}
bool MainWindow::ask_to_save()
{
    return false;

    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}


bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return false;
    }

    QTextStream out(&file);
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}
void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    //textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}
QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
#ifndef QT_NO_SESSIONMANAGER

void MainWindow::commitData(QSessionManager &manager)
{
    if (manager.allowsInteraction()) {
        if (!ask_to_save())
            manager.cancel();
    } else {

    }
}
#endif

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}
