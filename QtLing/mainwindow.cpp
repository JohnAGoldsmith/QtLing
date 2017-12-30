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

//typedef  QMap<QString,CWord*>                       StringToWordPtr;
//typedef  QPair<CStem*,CSignature*>                  stem_sig_pair;
//typedef  QPair<stem_sig_pair*,                      stem_sig_pair*> pair_of_stem_sig_pairs;
//typedef  QPair<QString, pair_of_stem_sig_pairs*>    five_tuple_sig_diffs;

class LxaStandardItemModel;







MainWindow::MainWindow()
{

    m_lexicon_list.append ( new CLexicon() );
    // models
    m_Models["Words"]                   = new LxaStandardItemModel("Words");
    m_Models["Stems"]                   = new LxaStandardItemModel("Stems");
    m_Models["Suffixes"]                = new LxaStandardItemModel("Suffixes");
    m_Models["Signatures"]              = new LxaStandardItemModel("Signatures");
    m_Models["EPositive Signatures"]    = new LxaStandardItemModel("EPositive Signatures");
    m_Models["Prefix signatures"]       = new LxaStandardItemModel("Prefix signatures");
    m_Models["EPositive Prefix Signatures"] = new LxaStandardItemModel("EPositive Prefix Signatures");
    m_Models["Residual parasignatures"] = new LxaStandardItemModel("Residual parasignatures");
    m_Models["SigGraphEdges"]            = new LxaStandardItemModel("SigTreeEdges");
    m_Models["Parasuffixes"]            = new LxaStandardItemModel("Parasuffixes");
    m_Models["Passive signatures"]      = new LxaStandardItemModel("Passive signatures");
    m_Models["Hypotheses"]              = new LxaStandardItemModel("Hypotheses");
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

    m_graphics_scene            = new lxa_graphics_scene (this);
    m_graphics_view             = new lxa_graphics_view(this);
    m_graphics_view->set_graphics_scene(m_graphics_scene);
    m_graphic_display_flag      = false;             // toggle with Ctrl-G



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

    connect(m_leftTreeView, SIGNAL(clicked(const QModelIndex&)), m_tableView_upper_left, SLOT(ShowModelsUpperTableView(const QModelIndex&)));
    connect(m_leftTreeView, SIGNAL(clicked(const QModelIndex&)), m_tableView_upper_right, SLOT(ShowModelsUpperTableView(const QModelIndex&)));
    connect(m_tableView_upper_left,SIGNAL(clicked(const QModelIndex & )), m_tableView_lower,SLOT(display_this_item(const QModelIndex &  )));
    connect(m_tableView_upper_right,SIGNAL(clicked(const QModelIndex & )), m_tableView_lower,SLOT(display_this_item(const QModelIndex &  )));
}
void MainWindow::cycle_through_graphic_displays()
{
    switch (m_graphic_display_type)
    {   qDebug() << "trying to cycle graphic display"<< 141;
        case DT_All_Suffix_Signatures:
            m_graphic_display_type = DT_Positive_Suffix_Signatures;
        case DT_Positive_Suffix_Signatures:
            m_graphic_display_type = DT_All_Prefix_Signatures;
        default:
            m_graphic_display_type = DT_All_Prefix_Signatures;
    }
}

void MainWindow::keyPressEvent(QKeyEvent* ke)
{
    if (ke->key() == Qt::Key_S){
        do_crab();
        display_suffix_signatures();
    }
    if (ke->key() == Qt::Key_D){
        read_dx1_file();
    }
    if (ke->key() == Qt::Key_P){
        get_lexicon()->set_prefixes_flag();
        do_crab();
    }
    if (ke->key() == Qt::Key_G)
    {
        if (m_graphic_display_flag==false){
            m_rightSplitter->replaceWidget(1,m_graphics_view);
            m_graphics_view->centerOn(0,1000);// this should be fixed so that the initial showing of the graphic is done right.
            m_graphic_display_flag = true;
            m_rightSplitter->setFocus();
            qDebug() << "trying to show graphics";
        } else{
            m_rightSplitter->replaceWidget(1,m_tableView_lower);
            m_graphic_display_flag = false;
        }
    }
    if (ke->key() == Qt::Key_2){
        do_crab2();
    }
    if (ke->key() == Qt::Key_V){
        cycle_through_graphic_displays();
    }
    if (ke->key() == Qt::Key_J){
        m_graphics_scene->move_rows_apart();
    }
    if (ke->key() == Qt::Key_K){
        m_graphics_scene->move_rows_closer();
    }
    if (ke->key() == Qt::Key_L){
        get_graphics_view()->move_down();
    }
    if (ke->key() == Qt::Key_Semicolon){
        qDebug() << "semicolon";
        get_graphics_view()->move_up();
    }
    if (ke->key() == Qt::Key_U){
        m_graphics_scene->widen_columns();
    }
    if (ke->key() == Qt::Key_I){
        m_graphics_scene->narrow_columns();
    }
    if (ke->key() == Qt::Key_A){
        qDebug() << 214 << "set focus sig";
        m_graphics_scene->set_focus_signature();

    }
    if (ke->key()== Qt::Key_H){

    }
    QMainWindow::keyPressEvent(ke);
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

void MainWindow::do_crab()
{   statusBar()->showMessage("Entering the Crab Nebula.");
    get_lexicon()->Crab_1();

    statusBar()->showMessage("We have returned from the Crab Nebula.");
    m_Models["Words"]               ->load_words(get_lexicon()->get_words());
    m_Models["Stems"]               ->load_stems(get_lexicon()->get_stems());
    m_Models["Suffixes"]            ->load_suffixes(get_lexicon()->get_suffixes());
    m_Models["Signatures"]          ->load_signatures(get_lexicon()->get_signatures());
    m_Models["EPositive Signatures"]->load_positive_signatures(get_lexicon()->get_signatures());
    m_Models["Prefix signatures"]   ->load_signatures(get_lexicon()->get_prefix_signatures());
    m_Models["EPositive Prefix Signatures"]->load_positive_signatures(get_lexicon()->get_prefix_signatures());
    m_Models["Residual parasignatures"]->load_parasignatures(get_lexicon()->get_residual_signatures());
    m_Models["Parasuffixes"]        ->load_suffixes(get_lexicon()->get_parasuffixes());
    m_Models["Passive signatures"]  ->load_signatures(get_lexicon()->get_passive_signatures());
    m_Models["Hypotheses"]          ->load_hypotheses(get_lexicon()->get_hypotheses());
    createTreeModel();

// add component 4
    //  part of an experiment:
    //     QMapIterator<QString,eComponentType> iter (get_lexicon()->get_category_types());
    //    while (iter.hasNext()){
    //      QString component_name = iter.next().key();
    //      eComponentType this_component_type = iter.value();
    //      m_Models[component_name]->load_category(component_name, this_component_type);
    //    }
    // end of experiment


        m_graphics_scene = new lxa_graphics_scene(this);

    m_graphics_view->setScene(m_graphics_scene);
    m_graphics_scene->set_graphics_view(m_graphics_view);
    m_leftTreeView->expandAll();
    m_leftTreeView->resizeColumnToContents(0);
    statusBar()->showMessage("All models are loaded.");



}
void MainWindow::do_crab2()
{   statusBar()->showMessage("Entering the Crab Nebula, phase 2");
    get_lexicon()->Crab_2();
    qApp->processEvents();
    statusBar()->showMessage("We have returned from the Crab Nebular again.");
    m_Models["Words"]               ->load_words(get_lexicon()->get_words());
    qApp->processEvents();
    statusBar()->showMessage("Loaded words.");


    m_Models["Stems"]               ->load_stems(get_lexicon()->get_stems());
    statusBar()->showMessage("Loaded stems.");

    m_Models["Suffixes"]            ->load_suffixes(get_lexicon()->get_suffixes());

    statusBar()->showMessage("Loading signatures.");
    qApp->processEvents();

    m_Models["Signatures"]          ->load_signatures(get_lexicon()->get_signatures());
    statusBar()->showMessage("Loaded signatures.");

    m_Models["EPositive Signatures"]    ->load_positive_signatures(get_lexicon()->get_signatures());
    m_Models["Prefix signatures"]   ->load_signatures(get_lexicon()->get_prefix_signatures());
    m_Models["EPositive Prefix Signatures"]    ->load_positive_signatures(get_lexicon()->get_prefix_signatures());

    statusBar()->showMessage("Loading signature-tree edges.");
    qApp->processEvents();

    m_Models["SigGraphEdges"]        ->load_sig_graph_edges(get_lexicon()->get_sig_graph_edge_map());

    statusBar()->showMessage("Loading residual signatures.");
    qApp->processEvents();

    m_Models["Residual parasignatures"]->load_parasignatures(get_lexicon()->get_residual_signatures());
    m_Models["Parasuffixes"]        ->load_suffixes(get_lexicon()->get_parasuffixes());
    m_Models["Passive signatures"]  ->load_signatures(get_lexicon()->get_passive_signatures());
    m_Models["Hypotheses"]          ->load_hypotheses(get_lexicon()->get_hypotheses());
    createTreeModel();

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

    if (m_graphics_scene == NULL){
        m_graphics_scene = new lxa_graphics_scene(this);
    }else{
        m_graphics_scene->clear_all();
    }

    if (get_lexicon()->get_suffix_flag()){
        m_graphics_scene->assign_scene_positions_to_signatures(get_lexicon()->get_signatures(),   DT_All_Suffix_Signatures);
    }else{
        m_graphics_scene->assign_scene_positions_to_signatures(get_lexicon()->get_prefix_signatures(),DT_All_Prefix_Signatures);
    }


    m_graphics_scene->place_signatures();
    m_graphics_view->setScene(m_graphics_scene);
    m_leftTreeView->expandAll();
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
            if (!file.open(QFile::ReadOnly | QFile::Text)) {
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

void MainWindow::createTreeModel()
{
    QStandardItem * parent = m_treeModel->invisibleRootItem();

//  this pair of lines must stay here after experiment:
    QStandardItem * lexicon_item = new QStandardItem(QString("Lexicon"));
    QStandardItem * lexicon_count_item = new QStandardItem(QString("1"));

// will be eliminated by the experiment:
    QStandardItem * word_item = new QStandardItem(QString("Words"));
    QStandardItem * word_count_item = new QStandardItem(QString::number(get_lexicon()->get_word_collection()->get_count()));

    QStandardItem * stem_item = new QStandardItem(QString("Stems"));
    QStandardItem * stem_count_item = new QStandardItem(QString::number(get_lexicon()->GetStemCollection()->get_count()));

    QStandardItem * suffix_item = new QStandardItem(QString("Suffixes"));
    QStandardItem * suffix_count_item = new QStandardItem(QString::number(get_lexicon()->GetSuffixCollection()->get_count()));

    QStandardItem * sig_item = new QStandardItem(QString("Signatures"));
    QStandardItem * sig_count_item = new QStandardItem(QString::number(get_lexicon()->GetSignatureCollection()->get_count()));

    QStandardItem * pos_sig_item = new QStandardItem(QString("EPositive Signatures"));
    QStandardItem * pos_sig_count_item = new QStandardItem(QString::number(get_lexicon()->GetSignatureCollection()->get_number_of_epositive_signatures()));

    QStandardItem * prefix_sig_item = new QStandardItem(QString("Prefix signatures"));
    QStandardItem * prefix_sig_count_item = new QStandardItem(QString::number(get_lexicon()->get_prefix_signatures()->get_count()));

    QStandardItem * pos_prefix_sig_item = new QStandardItem(QString("EPositive Prefix Signatures"));
    QStandardItem * pos_prefix_sig_count_item = new QStandardItem(QString::number(get_lexicon()->get_prefix_signatures()->get_number_of_epositive_signatures()));

    QStandardItem * residual_sig_item = new QStandardItem(QString("Residual parasignatures"));
    QStandardItem * residual_sig_count_item = new QStandardItem(QString::number(get_lexicon()->get_residual_signatures()->get_count()));

    QStandardItem * parasuffix_item = new QStandardItem(QString("Parasuffixes"));
    QStandardItem * parasuffix_count_item = new QStandardItem(QString::number(get_lexicon()->get_parasuffixes()->get_count()));

    QStandardItem * sig_graph_edge_item = new QStandardItem(QString("Signature graph edges"));
    QStandardItem * sig_graph_edge_count_item = new QStandardItem(QString::number(get_lexicon()->get_sig_graph_edge_map()->size()));

    QStandardItem * passive_signature_item = new QStandardItem(QString("Passive signatures"));
    QStandardItem * passive_signature_count_item = new QStandardItem(QString::number(get_lexicon()->get_passive_signatures()->get_count()));

    QStandardItem * hypothesis_item = new QStandardItem(QString("Hypotheses"));
    QStandardItem * hypothesis_count_item = new QStandardItem(QString::number(get_lexicon()->get_hypotheses()->count()));

// This is part of an experiment:
//  This code deals with the components in the Lexicon, so that that set can be easily updated by the programmer.
//  it will eliminate above code and below code too:
//
//          QList<QStandardItem*>                   lexicon_items;
//                                                  lexicon_items.append(lexicon_item);
//                                                  lexicon_items.append(lexicon_count_item);
    //                                              parent->appendRow(lexicon_items);
    //      QMapIterator<QString,eComponentType>    iter (get_lexicon()->get_category_types());
    //      while (iter.hasNext()){
    //
    //          QString             component_name = iter.next().key();
    //          eComponentType      this_component_type = iter.value();
    //          QList<QStandardItem*> this_list_of_standard_items;
    //
    //          QStandardItem *     component_item = new QStandardItem(component_name);
    //          QStandardItem *     component_count_item = new QStandardItem(component_count_name);
    //            this_list_of_standard_items.append(component_item);
    //            this_list_of_standard_items.append(component_count_item);
    //            lexicon_item.appendRow(this_list_of_standard_items);
    //    }
    // end of experiment




// add component 6

    QList<QStandardItem*> lexicon_items;
    lexicon_items.append(lexicon_item);
    lexicon_items.append(lexicon_count_item);

    QList<QStandardItem*> word_items;
    word_items.append(word_item);
    word_items.append(word_count_item);

    QList<QStandardItem*> stem_items;
    stem_items.append(stem_item);
    stem_items.append(stem_count_item);

    QList<QStandardItem*> suffix_items;
    suffix_items.append(suffix_item);
    suffix_items.append(suffix_count_item);

    QList<QStandardItem*> sig_items;
    sig_items.append(sig_item);
    sig_items.append(sig_count_item);

    QList<QStandardItem*> pos_sig_items;
    pos_sig_items.append(pos_sig_item);
    pos_sig_items.append(pos_sig_count_item);

    QList<QStandardItem*> prefix_sig_items;
    prefix_sig_items.append(prefix_sig_item);
    prefix_sig_items.append(prefix_sig_count_item);

    QList<QStandardItem*> pos_prefix_sig_items;
    pos_prefix_sig_items.append(pos_prefix_sig_item);
    pos_prefix_sig_items.append(pos_prefix_sig_count_item);

    QList<QStandardItem*> residual_sig_items;
    residual_sig_items.append(residual_sig_item);
    residual_sig_items.append(residual_sig_count_item);

    QList<QStandardItem*> parasuffix_items;
    parasuffix_items.append(parasuffix_item);
    parasuffix_items.append(parasuffix_count_item);

    QList<QStandardItem*> sig_graph_edge_items;
    sig_graph_edge_items.append(sig_graph_edge_item);
    sig_graph_edge_items.append(sig_graph_edge_count_item);

    QList<QStandardItem*> passive_signature_items;
    passive_signature_items.append(passive_signature_item);
    passive_signature_items.append(passive_signature_count_item);



    QList<QStandardItem*> hypothesis_items;
    hypothesis_items.append(hypothesis_item);
    hypothesis_items.append(hypothesis_count_item);

// add component 7

    parent->appendRow(lexicon_items);
    lexicon_item->appendRow(word_items);
    lexicon_item->appendRow(stem_items);
    lexicon_item->appendRow(suffix_items);
    lexicon_item->appendRow(sig_items);
    lexicon_item->appendRow(pos_sig_items);
    lexicon_item->appendRow(prefix_sig_items);
    lexicon_item->appendRow(pos_prefix_sig_items);
    lexicon_item->appendRow(sig_graph_edge_items);
    lexicon_item->appendRow(residual_sig_items);
    lexicon_item->appendRow(parasuffix_items);
    lexicon_item->appendRow(passive_signature_items);
    lexicon_item->appendRow(hypothesis_items);
// add component 8
}



/**
 * @brief MainWindow::display_suffix_signatures
 * This is called by a QAction.
 */
void MainWindow::display_suffix_signatures()
{
    m_tableView_upper_left->setModel(m_Models["Signatures"]);
    m_tableView_upper_left->set_document_type( SIGNATURES );
    m_tableView_upper_left->set_content_type( "signatures");
    CLexicon* lexicon = get_lexicon();
    m_graphics_scene->clear_all();
    m_graphics_scene->assign_scene_positions_to_signatures(lexicon->get_signatures(), DT_All_Suffix_Signatures);
    m_graphics_scene->place_signatures();
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

void MainWindow::sort_upper_table()
{
    // a signal comes to sort the contents of the upper table.
    if (m_tableView_upper_left->get_document_type()== SIGNATURES){
        if (m_tableView_upper_left->get_signature_sort_style()==SIG_BY_STEM_COUNT){
            m_tableView_upper_left->set_signature_sort_style(SIG_BY_AFFIX_COUNT);
        } else{
            m_tableView_upper_left->set_signature_sort_style(SIG_BY_STEM_COUNT);
        }
    }

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

    QAction * suffix_signature_display_action = new QAction();
    //connect(suffix_signature_display_action, &QAction::triggered, m_tableView_upper_left,  &UpperTableView::display_suffix_signatures );
    connect(suffix_signature_display_action, &QAction::triggered, this,  &MainWindow::display_suffix_signatures );

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
