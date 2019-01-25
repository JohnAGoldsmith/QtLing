#include "mainwindow.h"
//#include "generaldefinitions.h"
#include "ui_mainwindow.h"

#include "iostream"
#include <algorithm>
#include <stdio.h>

#include <QStandardItemModel>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QKeyEvent>
#include <QtWidgets>
#include <QString>
#include <QDebug>
#include <QPair>
#include <QMapIterator>
#include <QDir>
#include <Qt>
#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QWidget>
#include <QtWidgets>
#include <QTreeView>
#include <QKeyEvent>
#include <QSplitter>
#include <QTableView>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSortFilterProxyModel>
#include <algorithm>

#include "evaluation.h"
#include "generaldefinitions.h"
#include "graphics.h"
#include "lxamodels.h"
#include "mainwindow.h"
#include "mainwindow_find.h"
#include "mainwindow_menubar.h"
#include "string_group.h"
#include "StemCollection.h"
#include "Stem.h"
#include "SuffixCollection.h"
#include "Suffix.h"
#include "SignatureCollection.h"
#include "Signature.h"
#include "WordCollection.h"
#include "Word.h"

#include "table_views.h"

class LxaStandardItemModel;


MainWindow::MainWindow()
{
    m_my_lexicon = new CLexicon();
    m_lexicon_list.append (m_my_lexicon);
    CLexicon * lexicon =  m_my_lexicon;

    setFocusPolicy(Qt::StrongFocus); // allows it to capture keystrokes

    // models
    // See explanation of code below in comment in table_views_upper.cpp
    QString str_model_name;
    foreach (str_model_name, m_model_names) {
        //new_proxy_model->setSourceModel(m_Models[str_model_name]);
        m_Models[str_model_name] = new LxaStandardItemModel(str_model_name);
        m_proxy_models[str_model_name] = new LxaSortFilterProxyModel(this);
    }
    foreach (str_model_name, m_duplicate_model_names) {
        m_proxy_models[str_model_name] = new LxaSortFilterProxyModel(this);
    }
    /*
    m_Models["Words"]                       = new LxaStandardItemModel("Words");
    m_Models["Suffixal stems"]              = new LxaStandardItemModel("Suffixal stems");
    m_Models["Prefixal stems"]              = new LxaStandardItemModel("Prefixal stems");
    m_Models["Suffixes"]                    = new LxaStandardItemModel("Suffixes");
    m_Models["Prefixes"]                    = new LxaStandardItemModel("Prefixes");
    m_Models["Signatures"]                  = new LxaStandardItemModel("Signatures");
   // m_Models["Signatures 2"]                = new LxaStandardItemModel("Signatures");// sorted by affix count;
    //m_Models["Signatures 3"]                = new LxaStandardItemModel("Signatures");// used temporarily;
    m_Models["EPositive signatures"]        = new LxaStandardItemModel("EPositive signatures");
    //m_Models["EPositive signatures 2"]      = new LxaStandardItemModel("EPositive signatures 2"); // sorted by affix count
    m_Models["Prefix signatures"]           = new LxaStandardItemModel("Prefix signatures");
    //m_Models["Prefix signatures 2"]         = new LxaStandardItemModel("Prefix signatures"); //sorted by affix count;
    m_Models["EPositive prefix signatures"] = new LxaStandardItemModel("EPositive prefix signatures");
    //m_Models["EPositive prefix signatures 2"]= new LxaStandardItemModel("EPositive prefix signatures");
    m_Models["Residual parasignatures"]     = new LxaStandardItemModel("Residual parasignatures");
    m_Models["SigGraphEdges_1"]             = new LxaStandardItemModel("SigTreeEdges_1");
    m_Models["SigGraphEdges_2"]             = new LxaStandardItemModel("SigTreeEdges_2");
    m_Models["Parasuffixes"]                = new LxaStandardItemModel("Parasuffixes");
    m_Models["Passive signatures"]          = new LxaStandardItemModel("Passive signatures");
    m_Models["Hypotheses"]                  = new LxaStandardItemModel("Hypotheses");
    m_Models["Hypotheses 2"]                = new LxaStandardItemModel("Hypotheses 2");
    // for displaying protostems
    m_Models["Suffixal protostems"]         = new LxaStandardItemModel("Suffixal protostems");
    m_Models["Prefixal protostems"]         = new LxaStandardItemModel("Prefixal protostems");
    m_Models["Compound words"]              = new LxaStandardItemModel("Compound words");
    */

    m_treeModel     = new QStandardItemModel();

    get_lexicon()->set_status_bar(statusBar());

    // views
    m_leftTreeView                          = new LeftSideTreeView(this);
    m_tableView_upper_left                  = new UpperTableView (this);
    m_tableView_upper_right                 = new UpperTableView (this, SIG_BY_AFFIX_COUNT);
    m_tableView_lower                       = new LowerTableView (this);
    m_tableView_upper_left->setSortingEnabled(true);
    m_tableView_upper_right->setSortingEnabled(true);
    m_graphics_scene                        = new lxa_graphics_scene( this, lexicon);
    m_graphics_view                         = new lxa_graphics_view(m_graphics_scene, this);
    m_graphic_display_flag                  = false;             // toggle with Ctrl-G
    m_graphics_scene->set_signature_collection(get_lexicon()->get_signatures());

    // Status bars on top of each table view

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

    QWidget::setTabOrder(m_leftTreeView, m_tableView_upper_left);
    QWidget::setTabOrder(m_tableView_upper_left, m_tableView_upper_right);


    setCentralWidget(m_mainSplitter);

    m_ProgressBar  = new QProgressBar(this);
    m_ProgressBar->setVisible(true);
    statusBar()  ->addPermanentWidget(m_ProgressBar);
    get_lexicon()->set_progress_bar (m_ProgressBar);

    // Set dock-widget for find functionality, but not showing it yet
    m_find_dock_widget = new FindDockWidget(this);
    addDockWidget(Qt::BottomDockWidgetArea, m_find_dock_widget);
    m_find_dock_widget->setVisible(false);

    createActions(); // render the main menu
    createStatusBar();
    readSettings();

    // resize the main window
    resize(QDesktopWidget().availableGeometry(this).size() * 0.9);
    // set sizes of children of main splitter, i.e. left tree view and tables on the right
    m_mainSplitter->setSizes(QList<int>() << 800 <<2000);

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);


    setFocus(Qt::OtherFocusReason);


    // Qt SIGNAL-SLOT model that connects clicks in the left window to displays on the right side of the screen

    // clicking on certain items in the tree view displays tables on the upper left and upper right
    connect(m_leftTreeView, SIGNAL(clicked(const QModelIndex&)),
            m_tableView_upper_left, SLOT(ShowModelsUpperTableView(const QModelIndex&)));
    connect(m_leftTreeView, SIGNAL(clicked(const QModelIndex&)),
            m_tableView_upper_right, SLOT(ShowModelsUpperTableView(const QModelIndex&)));


    // clicking on the upperleft corner can signal a graphic view below it, or a table below it.
    connect(m_tableView_upper_left,SIGNAL(clicked(const QModelIndex & )),
            m_tableView_lower,SLOT(display_this_item(const QModelIndex &  )));

    connect(m_tableView_upper_right,SIGNAL(clicked(const QModelIndex & )),
            m_tableView_lower,SLOT(display_this_item(const QModelIndex &  )));

    connect(m_tableView_upper_left,SIGNAL(clicked(const QModelIndex & )),
            m_tableView_upper_right,SLOT(display_this_affixes_signatures(const QModelIndex &  )));

    /* Explanation for these signal-slot connections:
     * A signal is sent to the m_main_menu_bar object after the following
     * events are completed:
     * 1. lexicon_ready(): A lexicon is read into linguistica and analysed
     *    by pressing ctrl_S or ctrl_P (see MainWindow::do_crab())
     * 2. xml_parsed(): An Alchemist XML file is read into linguistica and
     *    successfully parsed (see MainWindow::gs_read_and_parse_xml())
     * 3. morfessor_parsed(): A Morfessor txt file that contains morphological
     *    cuts of words is read and parsed (see MainWindow::read_morfessor_txt_file())
     * Then menu bar records whether it has received these signals and will
     * change the availability of certain QActions depending on whether it has
     * received the signals, e.g. for the "Evaluate current lexicon using Gold
     * Standard" action to be enabled, the menu bar must have receive both the
     * "lexicon_ready" signal and the "xml_parsed" signal.
     */
    connect(this, SIGNAL(xml_parsed()), m_main_menu_bar, SLOT(gs_ready()));
    connect(this, SIGNAL(lexicon_ready()), m_main_menu_bar, SLOT(lexicon_ready()));
    connect(this, SIGNAL(morfessor_parsed()), m_main_menu_bar, SLOT(eval_parse_ready()));
}

void MainWindow::keyPressEvent(QKeyEvent* ke)
{   /*
     Keyboard shortcuts grapped by Actions:
     QKeySequence::Open
     QKeySequence::SaveAs
     QKeySequence::Quit
     QKeySequence::Cut
     QKeySequence::Paste
     QKeySequence::Find
     QKeySequence::Control S suffixes
     QKeySequence::Control P prefixes
     QKeySequence::Control C compounds
    */

    int this_key = ke->key();

    // the MainWindow only responds to keyboard instructions with Control Key depressed.
    if (ke->modifiers() != Qt::ControlModifier && ke->modifiers() != Qt::AltModifier){
        ke->ignore();
        return;
    }
    switch(this_key){
    case Qt::Key_1:
    {if (ke->modifiers() == Qt::ControlModifier)
    {
        if (get_lexicon()->get_prefixal_stems()->get_count() > 0){
            do_crab2_prefixes();
        }
        break;
     }
    }
    case Qt::Key_2:
    {if (ke->modifiers() == Qt::ControlModifier)
    {
        if (get_lexicon()->get_suffixal_stems()->get_count() > 0){
            do_crab2_suffixes();
        }
        break;
    }
    }
    case  Qt::Key_3:
    {if (ke->modifiers() == Qt::ControlModifier)
    {
        statusBar()->showMessage(tr("Read file."), 5000);
        ask_for_project_file();
        break;
    }
    }
    case Qt::Key_4:
    {if (ke->modifiers() == Qt::AltModifier)
    {
        read_corpus();
        analyze_corpus();
        break;
    }
    }
    case Qt::Key_5:
    {if (ke->modifiers() == Qt::ControlModifier)
        {
            MainWindow* new_window = new MainWindow();
            CLexicon* sublexicon = get_lexicon()->build_sublexicon(new_window);
            m_lexicon_list.append(sublexicon);
            new_window->do_crab1();
            if (sublexicon->get_suffix_flag()){
                new_window->display_epositive_suffix_signatures(sublexicon);
            } else{
                new_window->display_epositive_prefix_signatures(sublexicon);
            }

            new_window->resize(600, 400);
            new_window->setWindowTitle("Sublexicon");
            new_window->show();

            break;
        }
    }
    case Qt::Key_6:{
        get_lexicon()->set_prefixes_flag();
        break;
    }
    case Qt::Key_C:
    {if (ke->modifiers() == Qt::ControlModifier)
        {   qDebug() << "key c";
            get_lexicon()->step10_find_compounds();
        }
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
    case Qt::Key_H:
    {
        QMainWindow::keyPressEvent(ke);
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
    case Qt::Key_P:
    {
        // this has been moved to the "Actions" which can link themselves to the keyboard shortcuts.
        //do_crab1_prefixes();
        if (ke->modifiers() == Qt::AltModifier)
        {
                // NB: this could be done with signals/slots.
                m_tableView_upper_left ->showPrefixSignatures();
                m_tableView_upper_right->showPrefixSignatures();
                break;
        }
        break;

    }
    case Qt::Key_Q:
    {   // reset scene scale
        m_graphics_view->resetMatrix();
         break;
    }
    case Qt::Key_S:
    {   if (ke->modifiers() == Qt::ControlModifier)
        {   // this has been moved to the "Actions" which can link themselves easy to keyboard shutcuts.
            //do_crab1_suffixes();
            break;
        }
        else
        {
        if (ke->modifiers() == Qt::AltModifier)
        {
                // NB: this could be done with signals/slots.
                m_tableView_upper_left ->showSuffixSignatures();
                m_tableView_upper_right->showSuffixSignatures();
                break;
        }
        }
    }
    case Qt::Key_V:
    {
    }
    case Qt::Key_U:
    {
        m_graphics_scene->widen_columns();
        break;
    }
    case Qt::Key_W:
    {
      if (ke->modifiers() == Qt::AltModifier){
        // NB: this could be done with signals/slots.
        m_tableView_upper_left ->showWords();
        m_tableView_upper_right->showWords();
        }
        break;
    }
    case Qt::Key_Y:
    {   // toggle flag for sliding signature icons to the left when they become in-use.
        m_graphics_scene->toggle_signature_slide_flag();
        break;
    }
    case Qt::Key_Z:
    {   // toggle graphical circles versus real shapes

        if (m_graphics_scene->get_display_circles_for_signatures()){
            qDebug() << "setting circles flag false";
            m_graphics_scene = new lxa_graphics_scene(this, get_lexicon(), false);
            m_graphics_scene->set_signature_collection(get_lexicon()->get_active_signature_collection());
            display_suffix_signatures(get_lexicon());
            }
        else{
            qDebug() << "setting circles flag true";
            m_graphics_scene = new lxa_graphics_scene(this, get_lexicon(), true);
            m_graphics_scene->set_signature_collection(get_lexicon()->get_active_signature_collection());
            display_suffix_signatures(get_lexicon());
        }
        display_epositive_suffix_signatures(get_lexicon());
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
    case Qt::Key_Comma:
    {
        display_epositive_suffix_signatures(get_lexicon());
        break;
    }
    case Qt::Key_Period:
    {
        display_epositive_prefix_signatures(get_lexicon());
        break;
    }
    }
}

void MainWindow::do_crab1_suffixes()
{
    get_lexicon()->set_suffixes_flag();
    do_crab1();
    display_epositive_suffix_signatures(get_lexicon());
}
void MainWindow::do_crab1_prefixes()
{
        get_lexicon()->set_prefixes_flag();
    do_crab1();
    display_epositive_prefix_signatures(get_lexicon());
}
void MainWindow::do_crab2_suffixes()
{
    get_lexicon()->set_suffixes_flag();
    do_crab2();
    display_epositive_suffix_signatures(get_lexicon());
}
void MainWindow::do_crab2_prefixes()
{
    get_lexicon()->set_prefixes_flag();
    do_crab2();
    display_epositive_prefix_signatures(get_lexicon());
}
void MainWindow::do_crab1()
{   statusBar()->showMessage("Entering the Crab Nebula.");
    CLexicon* lexicon = get_lexicon();
    lexicon->Crab_1();
    load_models(get_lexicon());
    write_stems_and_words();
    statusBar()->showMessage("We have returned from the Crab Nebula.");

    create_or_update_TreeModel(get_lexicon());
    m_graphics_scene->set_signature_collection(get_lexicon()->get_active_signature_collection());
    m_leftTreeView->expandAll();
    m_leftTreeView->resizeColumnToContents(0);
    statusBar()->showMessage("All models are loaded.");
    emit lexicon_ready();
}

void MainWindow::do_crab2()
{
    statusBar()->showMessage("Entering the Crab Nebula, phase 2");
    CLexicon* lexicon = get_lexicon();
    lexicon->Crab_2();
    load_models(lexicon);
    statusBar()->showMessage("We have returned from the Crab Nebula.");

    create_or_update_TreeModel(lexicon);
    if (lexicon->get_suffix_flag())
        print_suffix_signatures();
    else
        print_prefix_signatures();
    m_leftTreeView->expandAll();
    m_leftTreeView->resizeColumnToContents(0);
    write_stems_and_words();
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
    // qDebug() << " ask for filename" ;
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
    qDebug() << "Loading models";
    // Because proxy models are updated dynamically whenever their source model
    // is updated, we want to unlink proxy models from source models for now
    // and relink them after source models are all loaded.
    // -- Hanson 11.2
    QString str_model_name;
    foreach (str_model_name, m_proxy_models.keys()) {
        m_proxy_models[str_model_name]->setSourceModel(nullptr);
    }

    statusBar()->showMessage("Loading models: Words");
    m_Models["Words"]               ->load_words(lexicon->get_words());
    QCoreApplication::processEvents();

    statusBar()->showMessage("Loading models: Stems");
    m_Models["Suffixal stems"]      ->load_stems(lexicon->get_suffixal_stems());
    QCoreApplication::processEvents();

    m_Models["Prefixal stems"]      ->load_stems(lexicon->get_prefixal_stems());
    QCoreApplication::processEvents();

    statusBar()->showMessage("Loading models: Affixes");
    m_Models["Suffixes"]            ->load_suffixes(lexicon->get_suffixes());
    QCoreApplication::processEvents();

    m_Models["Prefixes"]            ->load_prefixes(lexicon->get_prefixes());
    statusBar()->showMessage("Prefixes.");
    QCoreApplication::processEvents();

    statusBar()->showMessage("Loading models: Signatures");
    m_Models["Signatures"]          ->load_signatures(lexicon->get_signatures());
    QCoreApplication::processEvents();

    /* Using the sorting function of the proxy models, we do not need duplicate source models,
     * removed them to save some memory, Hanson 11.2
     */
    /*
    m_Models["Signatures 2"]         ->load_signatures(lexicon->get_signatures(), SIG_BY_AFFIX_COUNT);
    statusBar()->showMessage("Signatures 2.");
    QCoreApplication::processEvents();

    m_Models["Signatures 3"]         ->load_signatures(lexicon->get_signatures());
    statusBar()->showMessage("Signatures 3.");
    QCoreApplication::processEvents();
    */
    m_Models["EPositive signatures"]->load_positive_signatures(lexicon->get_signatures());

    /*
    m_Models["EPositive signatures 2"]->load_positive_signatures(lexicon->get_signatures(),SIG_BY_AFFIX_COUNT);
    statusBar()->showMessage("EPositive signatures 2.");
    */
    //QCoreApplication::processEvents();

    m_Models["Prefix signatures"]   ->load_signatures( lexicon->get_prefix_signatures());
    //m_Models["Prefix signatures 2"] ->load_signatures(lexicon->get_prefix_signatures(), SIG_BY_AFFIX_COUNT);
    m_Models["EPositive prefix signatures"]->load_positive_signatures(lexicon->get_prefix_signatures());
    //m_Models["EPositive prefix signatures 2"]->load_positive_signatures(lexicon->get_prefix_signatures(), SIG_BY_AFFIX_COUNT);
    QCoreApplication::processEvents();

    statusBar()->showMessage("Loading models: Parasignatures");
    m_Models["Residual parasignatures"]->load_parasignatures(lexicon->get_residual_signatures());
    m_Models["Parasuffixes"]        ->load_suffixes(lexicon->get_parasuffixes());
    //QCoreApplication::processEvents();
    m_Models["Passive signatures"]  ->load_signatures(lexicon->get_passive_signatures());
    statusBar()->showMessage("Loading models: Hypotheses");
    m_Models["Hypotheses"]          ->load_hypotheses(lexicon->get_hypotheses());
    m_Models["Hypotheses 2"]        ->load_hypotheses_2(lexicon->get_hypotheses());
    //QCoreApplication::processEvents();
    m_Models["SigGraphEdges_1"]        ->load_sig_graph_edges(lexicon->get_sig_graph_edge_map(),1);
    m_Models["SigGraphEdges_2"]        ->load_sig_graph_edges(lexicon->get_sig_graph_edge_map(),2);
    //QCoreApplication::processEvents();
    statusBar()->showMessage("Loading models: Protostems");
    m_Models["Suffixal protostems"]->load_protostems(lexicon->get_suffixal_protostems());
    m_Models["Prefixal protostems"]->load_protostems(lexicon->get_prefixal_protostems());
    //QCoreApplication::processEvents();
    statusBar()->showMessage("Loading models: Compound words");
    m_Models["Compound words"]->load_compounds(lexicon->get_compounds());
    //QCoreApplication::processEvents();
    qDebug() << "finished loading models";
    qDebug() << "loading proxy models";

    // Now all source models are loaded. Link them to proxy models.
    statusBar()->showMessage("Loading models: Finishing up");
    foreach (str_model_name, m_Models.keys()) {
        m_proxy_models[str_model_name]->setSourceModel(m_Models[str_model_name]);
    }
    // Link duplicate proxy models to their corresponding source models. Hanson 11.2
    m_proxy_models["Words 2"]->setSourceModel(m_Models["Words"]);
    m_proxy_models["Prefixes 2"]->setSourceModel(m_Models["Prefixes"]);
    m_proxy_models["Suffixes 2"]->setSourceModel(m_Models["Suffixes"]);
    m_proxy_models["Signatures 2"]->setSourceModel(m_Models["Signatures"]);
    m_proxy_models["Prefix signatures 2"]->setSourceModel(m_Models["Prefix signatures"]);
    m_proxy_models["EPositive signatures 2"]->setSourceModel(m_Models["EPositive signatures"]);
    m_proxy_models["EPositive prefix signatures 2"]->setSourceModel(m_Models["EPositive prefix signatures"]);

    qDebug() << "finished loading proxy models";
    statusBar()->showMessage("Finished loading models.");

}
void MainWindow::read_file_do_crab()
{       read_dx1_file();
        statusBar()->showMessage(tr("Ready"));
        do_crab1();
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
            //qDebug() << 486 << word<< 486;
            if (words.size()> 1) {
                pWord->SetWordCount(words[1].toInt());
                //if (words[1].toInt() > 1000)
                   // qDebug() << 489 << words[0] << words[1];
            }
     }
    Words->sort_word_list();
    setCurrentFile(m_name_of_data_file);
    statusBar()->showMessage(tr("File loaded"), 5000);
    QStringList word_pair;
    QString common;

    if (false){
    for (int i = 500; i < 600; i++){
        word_pair.clear();
        word_pair.append(Words->get_reverse_sort_list()->at(i-1));
        word_pair.append(Words->get_reverse_sort_list()->at(i));

        left_string_diff(word_pair,common);
        qDebug() << Words->get_reverse_sort_list()->at(i-1) << Words->get_reverse_sort_list()->at(i);
        qDebug() << word_pair.first() << word_pair.last() << common << 494;
    }
    }
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



void MainWindow::print_suffix_signatures()
{
    //CSignature* pSig;
    int count = 0;
    CStem *  pStem;
    QString filename = "swahili-suffix-signatures.txt";
    QFile file (filename);
    if (file.open(QIODevice::ReadWrite)){
        QTextStream stream( &file);
    QStringList labels;
    labels  << tr("signature") << "stem count" << "robustness"<< "fullness";
    CSignatureCollection * signatures = get_lexicon()->get_suffix_signatures();
    CSignature*         pSig;
    signatures->sort(SIG_BY_AFFIX_COUNT);
    double threshold = signatures->get_lexicon()->get_entropy_threshold_for_positive_signatures();
    for (int signo = 0; signo<signatures->get_count(); signo++)
    {   pSig = signatures->get_at_sorted(signo);
        if (pSig->get_stem_entropy() < threshold){continue;}
        stream << pSig->get_key()    << endl << endl;
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




void MainWindow::print_prefix_signatures()
{
    //CSignature* pSig;
    int count = 0;
    CStem *  pStem;
    QString filename = "swahili-prefix-signatures.txt";
    QFile file (filename);
    if (file.open(QIODevice::ReadWrite)){
        QTextStream stream( &file);
    QStringList labels;
    labels  << tr("signature") << "stem count" << "robustness"<< "fullness";
    CSignatureCollection * signatures = get_lexicon()->get_prefix_signatures();
    CSignature*         pSig;
    signatures->sort(SIG_BY_AFFIX_COUNT);
    double threshold = signatures->get_lexicon()->get_entropy_threshold_for_positive_signatures();
    for (int signo = 0; signo<signatures->get_count(); signo++)
    {   pSig = signatures->get_at_sorted(signo);
        if (pSig->get_stem_entropy() < threshold){continue;}
        stream << pSig->get_key()    << endl << endl;
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

void MainWindow::launch_find_dock()
{
    FindDialog* find_dialog = m_find_dock_widget->get_child_dialog();
    if (sender() == findLeftAct)
        find_dialog->set_search_selection(QString("Upper-left table"));
    else if (sender() == findRightAct)
        find_dialog->set_search_selection(QString("Upper-right table"));
    else
        find_dialog->set_search_selection(QString("Both upper tables"));
    m_find_dock_widget->setVisible(true);
}

/*!
 * \brief Opens up a dialogue to get directory for a Gold Standard xml file.
 * Parses that file and stores parse results in a GoldStandard object.
 */
void MainWindow::gs_read_and_parse_xml()
{
    CLexicon* lexicon = get_lexicon();

    QString file_name = QFileDialog::getOpenFileName(this,
                                                     "Choose a gold standard file to open",
                                                     QString(),
                                                     "XML Files (*.xml)");
    //qDebug() << 114 << "Goldstandard.cpp: xml file opened";
    if (!file_name.isEmpty()) {
        GoldStandard* gs = lexicon->new_goldstandard_from_xml(file_name);
        if(gs->read_XML()) {
            emit xml_parsed();
            qDebug() << 633 << "mainwindow.cpp: xml_parsed signal emitted";
        } else {
            lexicon->delete_goldstandard();
            qDebug() << 636 << "mainwindow.cpp: error in opening xml file";
        }
    } else {
        qDebug() << 639 << "mainwindow.cpp: file cannot be opened!";
    }
}

/*!
 * \brief Does evaluation on the current lexicon with a loaded and parsed
 * Gold Standard. Loads resulting parses and data from the evaluation into
 * models to be shown in the TableViews and TreeView.
 */
void MainWindow::gs_evaluate()
{
    CLexicon* lexicon = get_lexicon();
    bool eval_succeeded = lexicon->do_gs_evaluation();
    if (eval_succeeded) {
        GoldStandard* p_gs = lexicon->get_goldstandard();
        /*
        qDebug() << 616 << "Mainwindow.cpp: evaluation succeeded\n" ;
        qDebug() << "Precision: " << p_gs->get_total_precision()
                 << "Recall: " << p_gs->get_total_recall();
        */
        // create new model
        //m_Models["Gold Standard Words"] = new LxaStandardItemModel("Gold Standard Words");
        m_Models["True Positive Parses"] = new LxaStandardItemModel("True Positive Parses");
        m_Models["Gold Standard Parses"] = new LxaStandardItemModel("Gold Standard Parses");
        m_Models["Retrieved Parses"] = new LxaStandardItemModel("Retrieved Parses");
        //m_Models["Gold Standard Words"]->load_GSMap(p_gs->get_gold_standard_parses(), "Gold Standard Words");
        m_Models["True Positive Parses"]->load_parsemap_from_gs(p_gs, p_gs->get_true_positive_parses(), "True Positives");
        m_Models["Gold Standard Parses"]->load_parsemap_from_gs(p_gs, p_gs->get_gs_parses(), "Gold Standard");
        m_Models["Retrieved Parses"]->load_parsemap_from_gs(p_gs, p_gs->get_retrieved_parses(), "Retrieved");

        update_TreeModel_for_gs(lexicon);

        QCoreApplication::processEvents();

    } else {
        qDebug() << 663 << "Mainwindow.cpp: evaluation failed";
    }
}

/*!
 * \brief Opens up a dialogue to get directory for a Morfessor output txt file.
 * Parses that txt file and stores parse results in an EvalParses object.
 */
void MainWindow::read_morfessor_txt_file()
{
    CLexicon* lexicon = get_lexicon();

    QString file_name = QFileDialog::getOpenFileName(this,
                                                     "Choose a Morfessor output file to open",
                                                     QString(),
                                                     "Morfessor Output Files (*.txt)");
    //qDebug() << 114 << "Goldstandard.cpp: xml file opened";
    if (!file_name.isEmpty()) {
        EvalParses* eval = lexicon->new_eval_parses_from_txt(file_name);
        if (eval->read_morfessor_txt_file()) {
            emit morfessor_parsed();
            qDebug() << 685 << "mainwindow.cpp: successfully read in morfessor txt file";
        } else {
            lexicon->delete_eval_parses();
            qDebug() << 688 << "mainwindow.cpp: error in reading morfessor txt file";
        }
    } else {
        qDebug() << 691 << "mainwindow.cpp: file cannot be opened!";
    }
}

/*!
 * \brief Evaluates the morphological parses by Morfessor with the currently
 * loaded Gold Standard. Loads resulting parses and data from the evaluation into
 * models to be shown in the TreeView.
 */
void MainWindow::gs_evaluate_morfessor()
{
    CLexicon* lexicon = get_lexicon();
    bool eval_succeded = lexicon->do_gs_evaluation_on_eval_parses();
    if (eval_succeded) {
        EvalParses* p_eval = lexicon->get_eval_parses();
        qDebug() << 701 << "Mainwindow.cpp: evaluation of Morfessor txt file succeeded" ;
        qDebug() << "Precision: " << p_eval->get_total_precision()
                 << "Recall: " << p_eval->get_total_recall();
        update_TreeModel_for_eval(lexicon);
        QCoreApplication::processEvents();
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//        Infra- and super-structure
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
