#include "mainwindow.h"
//#include "generaldefinitions.h"
#include "ui_mainwindow.h"
#include <Qt>
#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QWidget>
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
#include "table_views.h"
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
#include "mainwindow_find.h"
#include "mainwindow_menubar.h"
#include "evaluation.h"
#include "string_group.h"

class LxaStandardItemModel;


MainWindow::MainWindow()
{
    m_my_lexicon = new CLexicon(this);
    m_lexicon_list.append (m_my_lexicon);
    CLexicon * lexicon =  m_my_lexicon;

    setFocusPolicy(Qt::StrongFocus); // allows it to capture keystrokes

    //---------------------------------------------------------------------------------------------------//
    // models : this is old, from 2017 or so; I am redoing this, dec 2023 -- jan 2024
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
    // ---------------- end of old stuff to be removed eventually ----------------------------------//

    // each model needs two proxy models so its data can be shown in two different ways in two TableViews at the same time

    m_word_model = nullptr;
    m_word_model_proxy_1 = new wordSFProxymodel(this);
    m_word_model_proxy_2 = new wordSFProxymodel(this);

    /* We set the wordmodel at the point where we read the corpus, whether text or wordlist
    *  And we need to reset it when we do an analysis, so that the View knows how many parses there are for each word
    */


    m_suffix_signature_model = new signaturemodel(m_my_lexicon->get_signatures());
    m_prefix_signature_model = new signaturemodel (m_my_lexicon->get_prefix_signatures());
    m_signature_model_proxy_1 = new signatureSFProxymodel(this);
    m_signature_model_proxy_2 = new signatureSFProxymodel(this);

    m_suffixal_stem_model = nullptr;
    m_prefixal_stem_model = nullptr;
    m_stem_model_proxy_1 = new stemSFProxymodel(this);
    m_stem_model_proxy_2 = new stemSFProxymodel(this);

    m_suffix_model = nullptr;                           //new affixmodel(m_my_lexicon->get_suffixes()->get_suffix_list());
    m_prefix_model = nullptr;                           //new affixmodel(m_my_lexicon->get_prefixes()->get_prefix_list());

    m_affix_model_proxy_1 = new affixSFProxymodel (this);
    m_affix_model_proxy_2 = new affixSFProxymodel (this);

    m_suffixal_protostem_model = nullptr;
    m_suffixal_sigpair_model = nullptr;

    m_parse_model = nullptr;


    // -----------------  old --------------------------------------------------------------
    m_proxy_models["suffix_signatures_1"] = new LxaSortFilterProxyModel(this);
    m_proxy_models["suffix_signatures_2"] = new LxaSortFilterProxyModel(this);
    m_proxy_models["suffix_signatures_1"]->setSourceModel(m_suffix_signature_model);
    m_proxy_models["suffix_signatures_2"]->setSourceModel(m_suffix_signature_model);
    // --------------------------------------------------------------------------------------´


    m_treeModel     = new QStandardItemModel();

    get_lexicon()->set_status_bar(statusBar());

    // views
    m_leftTreeView                          = new LeftSideTreeView(this);
    m_tableView_upper_temp                  = new TableView(this); //UpperTableView (this);
    m_tableView_upper_left                  = new UpperTableView (this);
    m_tableView_upper_right                 = new UpperTableView (this, SIG_BY_AFFIX_COUNT);
    m_tableView_lower                       = new LowerTableView (this);
    m_tableView_upper_temp->setSortingEnabled(true);
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
    m_top_rightSplitter->addWidget(m_tableView_upper_temp);
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

    //resize(QDesktopWidget().availableGeometry(this).size() * 0.9);
    resize(QSize(1000,1000));

    m_mainSplitter->setSizes(QList<int>() << 800 <<2000);
    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);
    setFocus(Qt::OtherFocusReason);

     connect(m_leftTreeView, SIGNAL(clicked(QModelIndex)),
            m_tableView_upper_left, SLOT(ShowModelsUpperTableView(QModelIndex)));

    connect (m_tableView_upper_temp, SIGNAL(clicked(QModelIndex)),
            m_tableView_lower,SLOT(display_this_item(QModelIndex)));

    connect(m_tableView_upper_left,SIGNAL(clicked(QModelIndex)),
            m_tableView_lower,SLOT(display_this_item(QModelIndex)));

    connect(m_tableView_upper_right,SIGNAL(clicked(QModelIndex)),
            m_tableView_lower,SLOT(display_this_item(QModelIndex)));

    connect(m_tableView_upper_left,SIGNAL(clicked(QModelIndex)),
            m_tableView_upper_right,SLOT(display_this_affixes_signatures(QModelIndex)));

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

    case Qt::Key_2:
    {   if (ke->modifiers() == Qt::ControlModifier)
        {
            do_crab2();
        }
        break;
    }
    case Qt::Key_3:
    {
        if (ke->modifiers() == Qt::ControlModifier)
        {
            if (get_lexicon()->get_suffix_flag()){
              if (get_lexicon()->get_suffixal_stems()->get_count() > 0){
                  if (get_lexicon()->get_suffix_signatures()->get_count() > 0){
                       do_crab3();
                  }
              }
            }
            else{
                if (get_lexicon()->get_prefixal_stems()->get_count() > 0){
                    if (get_lexicon()->get_prefix_signatures()->get_count() > 0){
                         do_crab3();
                    }
                }
            }
         }
         break;
    }
    case Qt::Key_4:
    {
        if (ke->modifiers() == Qt::ControlModifier)
        {
            if (get_lexicon()->get_suffix_flag()){
              if (get_lexicon()->get_suffixal_stems()->get_count() > 0){
                  if (get_lexicon()->get_suffix_signatures()->get_count() > 0){
                       do_crab4();
                  }
              }
            }
            else{
                if (get_lexicon()->get_prefixal_stems()->get_count() > 0){
                    if (get_lexicon()->get_prefix_signatures()->get_count() > 0){
                         do_crab4();
                    }
                }
            }
         }
         break;
    }
    case Qt::Key_5:
    {
        if (ke->modifiers() == Qt::ControlModifier)
        {
            if (get_lexicon()->get_suffix_flag()){
              if (get_lexicon()->get_suffixal_stems()->get_count() > 0){
                  if (get_lexicon()->get_suffix_signatures()->get_count() > 0){
                       do_crab5();
                  }
              }
            }
            else{
                if (get_lexicon()->get_prefixal_stems()->get_count() > 0){
                    if (get_lexicon()->get_prefix_signatures()->get_count() > 0){
                         do_crab5();
                    }
                }
            }
         }
         break;
    }
    case  Qt::Key_6:
    {if (ke->modifiers() == Qt::ControlModifier)
        {
           do_crab6();
        }
        break;
    }
    case Qt::Key_7:
    {if (ke->modifiers() == Qt::ControlModifier)
        {
            MainWindow* new_window = new MainWindow();
            CLexicon* sublexicon = get_lexicon()->build_sublexicon(new_window);
            m_lexicon_list.append(sublexicon);
            new_window->do_crab1();
            if (sublexicon->get_suffix_flag()){
                new_window->display_epositive_suffix_signatures(sublexicon);
            } else {
                new_window->display_epositive_prefix_signatures(sublexicon);
            }

            new_window->resize(600, 400);
            new_window->setWindowTitle("Sublexicon");
            new_window->show();

            break;
        }
        break;
    }
    case Qt::Key_8:{
        get_lexicon()->set_prefixes_flag();
        break;
    }
    case Qt::Key_9:{
        get_lexicon()->step11_find_allomorphic_signatures();
        break;
    }
    case Qt::Key_C:
    {if (ke->modifiers() == Qt::ControlModifier)
        {
            do_crab_compounds();
        }
        break;
    }
    // __input
    case Qt::Key_D:     {
        qDebug() << "we hit key d";
        read_previous_data_file();
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
    case Qt::Key_O:{
        ask_for_filename();
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
        //m_graphics_view->resetMatrix();  TODO: What was this for? this idiom is deprecated anyway.
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
        break;
        }
    }
    // __input
    case Qt::Key_T: // read text (not word list, not dx1)
    {
    }
    case Qt::Key_V:
    {
        break;
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
void MainWindow::do_MDL()
{

    get_lexicon()->compute_MDL();
}
void MainWindow::do_crab1_suffixes()
{
    get_lexicon()->set_suffixes_flag();
    do_crab1();
}
void MainWindow::do_crab1_prefixes()
{
    get_lexicon()->set_prefixes_flag();
    do_crab1();

}
void MainWindow::do_crab1()
{   statusBar()->showMessage("Crab, phase 1.");
    CLexicon* lexicon = get_lexicon();
    lexicon->Crab_1();
    load_models(get_lexicon());
    write_stems_and_words();
    create_or_update_TreeModel(get_lexicon());
    m_graphics_scene->set_signature_collection(get_lexicon()->get_active_signature_collection());
    m_leftTreeView->expandAll();
    m_leftTreeView->resizeColumnToContents(0);
    get_lexicon()->get_suffix_flag()?
          display_epositive_suffix_signatures(get_lexicon()):
          display_epositive_prefix_signatures(get_lexicon());
    statusBar()->showMessage("Crab, phase 1 completed.");
}

void MainWindow::do_crab2()
{   statusBar()->showMessage("Crab 2: Good sigs inside bad.");
    CLexicon* lexicon = get_lexicon();
    lexicon->Crab_2();
    load_models(get_lexicon());
    write_stems_and_words();
    create_or_update_TreeModel(get_lexicon());
    m_graphics_scene->set_signature_collection(get_lexicon()->get_active_signature_collection());
    m_leftTreeView->expandAll();
    m_leftTreeView->resizeColumnToContents(0);
    get_lexicon()->get_suffix_flag()?
          display_epositive_suffix_signatures(get_lexicon()):
          display_epositive_prefix_signatures(get_lexicon());
    statusBar()->showMessage("Crab 2: Good sigs inside bad completed.");
    emit lexicon_ready();
}
void MainWindow::do_crab3()
{   statusBar()->showMessage("Crab Nebula, phase 3: repair low entropy signatures.");
    CLexicon* lexicon = get_lexicon();
    lexicon->Crab_3();
    load_models(lexicon);
    create_or_update_TreeModel(lexicon);
    if (lexicon->get_suffix_flag())
        print_suffix_signatures();
    else
        print_prefix_signatures();
    m_leftTreeView->expandAll();
    m_leftTreeView->resizeColumnToContents(0);
    write_stems_and_words();
    get_lexicon()->get_suffix_flag()?
          display_epositive_suffix_signatures(get_lexicon()):
          display_epositive_prefix_signatures(get_lexicon());
    statusBar()->showMessage("Crab 3: repair low entropy signatures completed.");
}
void MainWindow::do_crab4()
{   statusBar()->showMessage("Crab 4: split complex morphemes.");
    CLexicon* lexicon = get_lexicon();
    lexicon->Crab_4();
    load_models(lexicon);
    create_or_update_TreeModel(lexicon);
    if (lexicon->get_suffix_flag())
        print_suffix_signatures();
    else
        print_prefix_signatures();
    m_leftTreeView->expandAll();
    m_leftTreeView->resizeColumnToContents(0);
    write_stems_and_words();
    get_lexicon()->get_suffix_flag()?
          display_epositive_suffix_signatures(get_lexicon()):
          display_epositive_prefix_signatures(get_lexicon());
    statusBar()->showMessage("Crab 4: split complex morphemes completed.");
}
void MainWindow::do_crab5(){
    statusBar()->showMessage("Crab 5: simple compounds...");
        CLexicon* lexicon = get_lexicon();
        lexicon->Crab_5();
        load_models(lexicon);
        create_or_update_TreeModel(lexicon);
        m_leftTreeView->expandAll();
        m_leftTreeView->resizeColumnToContents(0);
        statusBar()->showMessage("Crab 5: simple compounds completed.");
}
void MainWindow::do_crab_compounds(){
    statusBar()->showMessage("Crab compounds: simple compounds...");
        CLexicon* lexicon = get_lexicon();
        lexicon->Crab_compounds();
        load_models(lexicon);
        create_or_update_TreeModel(lexicon);
        m_leftTreeView->expandAll();
        m_leftTreeView->resizeColumnToContents(0);
        statusBar()->showMessage("Crab compounds: simple compounds completed.");
}
void MainWindow::do_crab6(){
    statusBar()->showMessage("Crab 6 not defined.");
}
void MainWindow::newFile()
{
    if (ask_to_save()) {
        setCurrentFile(QString());
    }
}


void MainWindow::ask_for_project_file()
{
    m_name_of_project_file = QFileDialog::getOpenFileName (this);
    read_stems_and_words();

    display_suffix_signatures(get_lexicon());
}

// Dec 2023
void MainWindow::display_models(const QModelIndex & index){
    qDebug() << 647 << "display models";
    QString component;
    if (index.isValid()){
        component = index.data().toString();
    } else return;
    if (component == "Words")               { display_words();                           return;}
    if (component == "Suffix signatures")   { display_suffix_signatures(m_my_lexicon);   return;}
    if (component == "Prefix signatures")   { display_prefix_signatures(m_my_lexicon);   return;}
    if (component == "Suffix stems")        { display_suffix_stems(m_my_lexicon);        return;}
    if (component == "Prefixal stems")      { display_prefix_stems(m_my_lexicon);        return;}
    if (component == "Suffixal protostems") { display_suffixal_protostems(m_my_lexicon); return;} // this currently doesn´t happen
    if (component == "Suffixes")            { display_suffixes(m_my_lexicon);            return;}
    if (component == "Prefixes")            { display_prefixes(m_my_lexicon);            return;}
    if (component == "Parasuffixes")        { display_parasuffixes(m_my_lexicon);        return;}
    if (component == "Suffixal sig pairs")   { display_suffixal_sigpairs(m_my_lexicon);  return;}
    if (component == "Parses")              { display_parses(m_my_lexicon);              return;}
}



// this will be eliminated, since we keep the data inside the regular Linguistica containers -- not in the models
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

    // removed these Feb 20 2024; these duplicate use of memory, compared to models which don't replicate the entries.

    statusBar()->showMessage("Loading models: Parses");
    m_Models["Parses"]              ->load_parses(lexicon->get_parses());

    //statusBar()->showMessage("Loading models: Suffixal stems");
    //m_Models["Suffixal stems"]      ->load_stems(lexicon->get_suffixal_stems());
    //statusBar()->showMessage("Loading models: Prefixal stems");
    //m_Models["Prefixal stems"]      ->load_stems(lexicon->get_prefixal_stems());
    //statusBar()->showMessage("Loading models: Suffixes");
    //m_Models["Suffixes"]            ->load_suffixes(lexicon->get_suffixes());



    statusBar()->showMessage("Loading models: Prefixes");
    m_Models["Prefixes"]            ->load_prefixes(lexicon->get_prefixes());
    statusBar()->showMessage("Loading models: Signatures");


    //m_Models["Suffixal signatures"]          ->load_signatures(lexicon->get_signatures());
    //statusBar()->showMessage("Loading models: Signatures completed");




    /* Using the sorting function of the proxy models, we do not need duplicate source models,
     * removed them to save some memory, Hanson 11.2
     */
    statusBar()->showMessage("Loading models: EPositive signatures");
    m_Models["EPositive signatures"]->load_positive_signatures(lexicon->get_signatures());

    //m_Models["Prefix signatures"]   ->  load_signatures( lexicon->get_prefix_signatures());
    m_Models["EPositive prefix signatures"]->load_positive_signatures(lexicon->get_prefix_signatures());
    //m_Models["Residual parasignatures"]->load_parasignatures(lexicon->get_residual_signatures()); // problem -- these are too big!
    //m_Models["Parasuffixes"]        ->load_parasuffixes(lexicon->get_parasuffixes()); // these are continuations after any protostem.
    //m_Models["Passive signatures"]  ->load_signatures(lexicon->get_passive_signatures());
    statusBar()->showMessage("Loading models: Hypotheses");
    m_Models["Hypotheses"]          ->load_hypotheses(lexicon->get_hypotheses());
    m_Models["Hypotheses 2"]        ->load_hypotheses_2(lexicon->get_hypotheses());
    //m_Models["SigGraphEdges_1"]        ->load_sig_graph_edges(lexicon->get_sig_pairs()->get_map());
    //m_Models["SigGraphEdges_2"]        ->load_sig_graph_edges(lexicon->get_sig_pairs->get_map(),2);

    statusBar()->showMessage("Loading models: Suffixal protostems");
    //m_Models["Suffixal protostems"]->load_protostems(lexicon->get_suffixal_protostems());
    //m_Models["Prefixal protostems"]->load_protostems(lexicon->get_prefixal_protostems());
    m_Models["Compound words"]->load_compounds(lexicon->get_compounds());

    // Now all source models are loaded. Link them to proxy models.
    //statusBar()->showMessage("Loading models: Finishing up");
    foreach (str_model_name, m_Models.keys()) {
        m_proxy_models[str_model_name]->setSourceModel(m_Models[str_model_name]);
    }
    // Link duplicate proxy models to their corresponding source models. Hanson 11.2

    statusBar()->showMessage("Loading models: proxymodels");
    m_proxy_models["Words 2"]->setSourceModel(m_Models["Words"]);
    m_proxy_models["Prefixes 2"]->setSourceModel(m_Models["Prefixes"]);
    m_proxy_models["Suffixes 2"]->setSourceModel(m_Models["Suffixes"]);
    m_proxy_models["Signatures 2"]->setSourceModel(m_Models["Suffixal signatures"]);
    m_proxy_models["Prefix signatures 2"]->setSourceModel(m_Models["Prefix signatures"]);
    m_proxy_models["EPositive signatures 2"]->setSourceModel(m_Models["EPositive signatures"]);
    m_proxy_models["EPositive prefix signatures 2"]->setSourceModel(m_Models["EPositive prefix signatures"]);
    statusBar()->showMessage("Finished loading models.");
    qDebug() << "all models loaded";

}


// __input
void MainWindow::ask_for_filename()
{   QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    m_name_of_data_file = QFileDialog::getOpenFileName(this);
    QFile file(m_name_of_data_file);
    QString previous_data_file;
    previous_data_file = settings.value( "data_file" ).toString() ;
    if ( previous_data_file != m_name_of_data_file){
        settings.setValue("name_of_previous_data_file", m_name_of_data_file);
        settings.setValue("name_of_data_file", m_name_of_data_file );
    }
    setCurrentFile(m_name_of_data_file);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
           QMessageBox::warning(this, tr("Application"),
                                tr("Cannot read file %1:\n%2.")
                                .arg(QDir::toNativeSeparators(m_name_of_data_file), file.errorString()));
           return;
    }
    settings.setValue("data_file", m_name_of_data_file);
    QTextStream instream(&file);
    if (m_name_of_data_file.endsWith(".dx1")){
        read_dx1_file(instream);
    } else{
        read_text_file(instream);
    }
}
void MainWindow::read_previous_data_file(){
     QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
     m_name_of_data_file = settings.value("data_file").toString();
     QFile file(m_name_of_data_file);
     if (!file.open(QFile::ReadOnly | QFile::Text))
     {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(m_name_of_data_file), file.errorString()));
            return;
     }
     QTextStream in_stream(&file);
     if (m_name_of_data_file.endsWith(".dx1")){
         read_dx1_file(in_stream);
     } else{
         read_text_file(in_stream);
     }
}
// __input

void MainWindow::read_dx1_file (QTextStream & in_stream){
    QMap<QString, int> word_counts;
    while (!in_stream.atEnd()){
        QStringList items = in_stream.readLine().simplified().split(" ");
        QString word = items[0];
        if (items.size()> 1) {
            bool ok;
            int count = items[1].toInt(&ok);
            if (ok) {
                    word_counts[word] = count;
            } else{
                word_counts[word] = 1;
            }
        } else{
            word_counts[word] = 1;
        }
    }
    get_lexicon()->input_words(word_counts);
    if (m_word_model){delete m_word_model;}
    m_word_model = new wordmodel(get_lexicon()->get_word_collection());
    m_word_model_proxy_1 ->setSourceModel(m_word_model);
    m_word_model_proxy_2 ->setSourceModel(m_word_model);
}
void MainWindow::read_text_file (QTextStream & in_stream){
    QMap<QString, int> word_counts;
    while (!in_stream.atEnd()){
        QStringList items = in_stream.readLine().simplified().split(" ");
        foreach (QString word, items){
            if (word_counts.contains(word)){
                word_counts[word] += 1;
            } else{
                word_counts[word] = 1;
            }
        }
    }
    get_lexicon()->input_words(word_counts);
    if (m_word_model){delete m_word_model;}
    m_word_model = new wordmodel(get_lexicon()->get_word_collection());
    m_word_model_proxy_1 ->setSourceModel(m_word_model);
    m_word_model_proxy_2 ->setSourceModel(m_word_model);
}


// __input

void MainWindow::read_file_do_crab()
{
}


//__output
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
    int count = 0;
    CStem *  pStem;
    QString filename = "swahili-suffix-signatures.txt";
    QFile file (filename);
    if (file.open(QIODevice::ReadWrite)){
        QTextStream textstream1( &file);
    QStringList labels;
    labels  << tr("signature") << "stem count" << "robustness"<< "fullness";
    CSignatureCollection * signatures = get_lexicon()->get_suffix_signatures();
    CSignature*         pSig;
    signatures->sort(SIG_BY_AFFIX_COUNT);
    double threshold = signatures->get_lexicon()->get_entropy_threshold_for_positive_signatures();
    for (int signo = 0; signo<signatures->get_count(); signo++)
    {   pSig = signatures->get_at_sorted(signo);
        if (pSig->get_stem_entropy() < threshold){continue;}
        textstream1 << pSig->get_key()    << Qt::endl << Qt::endl;
        CStem_ptr_list_iterator stem_iter (*pSig->get_stems());
        while (stem_iter.hasNext()){
            pStem = stem_iter.next();
            textstream1 << pStem->get_key() << "\t";
            count++;
            if (count ==5){
                count = 0;
                textstream1 << Qt::endl;
            }
        }
        textstream1 << Qt::endl << Qt::endl;
        count = 0;
     }
     textstream1 << Qt::endl;
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
        QTextStream textstream1( &file);
    QStringList labels;
    labels  << tr("signature") << "stem count" << "robustness"<< "fullness";
    CSignatureCollection * signatures = get_lexicon()->get_prefix_signatures();
    CSignature*         pSig;
    signatures->sort(SIG_BY_AFFIX_COUNT);
    double threshold = signatures->get_lexicon()->get_entropy_threshold_for_positive_signatures();
    for (int signo = 0; signo<signatures->get_count(); signo++)
    {   pSig = signatures->get_at_sorted(signo);
        if (pSig->get_stem_entropy() < threshold){continue;}
        textstream1 << pSig->get_key()    << Qt::endl << Qt::endl;
        CStem_ptr_list_iterator stem_iter (*pSig->get_stems());
        while (stem_iter.hasNext()){
            pStem = stem_iter.next();
            textstream1 << pStem->get_key() << "\t";
            count++;
            if (count ==5){
                count = 0;
                textstream1 << Qt::endl;
            }
        }
        textstream1 << Qt::endl << Qt::endl;
        count = 0;
     }
     textstream1 << Qt::endl;
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

// __input
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
        //const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        QRect availableGeometry = QRect(0,0,1000,1000);
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

    QTextStream out_corpus_costs(&file);
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
