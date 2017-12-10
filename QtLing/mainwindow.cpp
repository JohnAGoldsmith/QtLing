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
#include <QDebug>
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
    m_Models["Prefix signatures"]       = new LxaStandardItemModel("Prefix signatures");
    m_Models["Residual parasignatures"] = new LxaStandardItemModel("Residual parasignatures");
    m_Models["SigTreeEdges"]            = new LxaStandardItemModel("SigTreeEdges");
    m_Models["Parasuffixes"]            = new LxaStandardItemModel("Parasuffixes");
    m_Models["Passive signatures"]      = new LxaStandardItemModel("Passive signatures");

    m_treeModel     = new QStandardItemModel();

    get_lexicon()->set_status_bar(statusBar());

    // views
    m_leftTreeView      = new LeftSideTreeView(this);
    m_tableView_upper   = new UpperTableView (this);
    m_tableView_lower   = new LowerTableView (this);
    m_tableView_upper->setSortingEnabled(true);

    m_graphics_scene = new lxa_graphics_scene (this, get_lexicon()->get_signatures());
    m_graphics_view  = new lxa_graphics_view(this);
    m_graphics_view->set_graphics_scene(m_graphics_scene);
    m_graphic_display_flag = false;             // toggle with Ctrl-G

    // set model for tree view
    m_leftTreeView->setModel(m_treeModel);
    m_leftTreeView->setColumnWidth(1,100);
    m_leftTreeView->setColumnWidth(0,1500);

    // layout
    m_mainSplitter = new QSplitter();
    m_rightSplitter = new QSplitter(Qt::Vertical);   
    m_rightSplitter->addWidget(m_tableView_upper);
    m_rightSplitter->addWidget(m_tableView_lower);
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

    connect(m_leftTreeView, SIGNAL(clicked(const QModelIndex&)), m_tableView_upper, SLOT(ShowModelsUpperTableView(const QModelIndex&)));
    connect(m_tableView_upper,SIGNAL(clicked(const QModelIndex & )), m_tableView_lower,SLOT(display_this_item(const QModelIndex &  )));

}


void MainWindow::keyPressEvent(QKeyEvent* ke)
{
    if (ke->key() == Qt::Key_S){
        do_crab();
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
            m_graphic_display_flag = true;
            m_rightSplitter->setFocus();
        } else{
            m_rightSplitter->replaceWidget(1,m_tableView_lower);
            m_graphic_display_flag = false;
        }
    }
    if (ke->key() == Qt::Key_2){
        do_crab2();
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

    statusBar()->showMessage("We have returned from the Crab Nebular.");
    m_Models["Words"]               ->load_words(get_lexicon()->get_words());
    m_Models["Stems"]               ->load_stems(get_lexicon()->get_stems());
    m_Models["Suffixes"]            ->load_suffixes(get_lexicon()->get_suffixes());
    m_Models["Signatures"]          ->load_signatures(get_lexicon()->get_signatures());
    m_Models["Prefix signatures"]   ->load_signatures(get_lexicon()->get_prefix_signatures());
    m_Models["Residual parasignatures"]->load_parasignatures(get_lexicon()->get_residual_signatures());
    m_Models["Parasuffixes"]        ->load_suffixes(get_lexicon()->get_parasuffixes());
    m_Models["Passive signatures"]  ->load_signatures(get_lexicon()->get_passive_signatures());
    createTreeModel();

    delete m_graphics_scene;
    if (get_lexicon()->get_suffix_flag()){
        m_graphics_scene = new lxa_graphics_scene(this, get_lexicon()->get_signatures());
    }else{
        m_graphics_scene = new lxa_graphics_scene(this, get_lexicon()->get_prefix_signatures());
    }
    m_graphics_view->setScene(m_graphics_scene);
    m_graphics_scene->set_graphics_view(m_graphics_view);


    m_leftTreeView->expandAll();
    statusBar()->showMessage("All models are loaded.");

}
void MainWindow::do_crab2()
{   statusBar()->showMessage("Entering the Crab Nebula, phase 2");
    get_lexicon()->Crab_2();
    qApp->processEvents();
    statusBar()->showMessage("We have returned from the Crab Nebular again.");
    m_Models["Words"]               ->load_words(get_lexicon()->get_words());

    statusBar()->showMessage("Loading stems.");
    qApp->processEvents();

    m_Models["Stems"]               ->load_stems(get_lexicon()->get_stems());
    m_Models["Suffixes"]            ->load_suffixes(get_lexicon()->get_suffixes());

    statusBar()->showMessage("Loading signatures.");
    qApp->processEvents();

    m_Models["Signatures"]          ->load_signatures(get_lexicon()->get_signatures());
    m_Models["Prefix signatures"]   ->load_signatures(get_lexicon()->get_prefix_signatures());

    statusBar()->showMessage("Loading signature-tree edges.");
    qApp->processEvents();

    m_Models["SigTreeEdges"]        ->load_sig_tree_edges(get_lexicon()->get_sig_tree_edge_map());

    statusBar()->showMessage("Loading residual signatures.");
    qApp->processEvents();

    m_Models["Residual parasignatures"]->load_parasignatures(get_lexicon()->get_residual_signatures());
    m_Models["Parasuffixes"]        ->load_suffixes(get_lexicon()->get_parasuffixes());
    m_Models["Passive signatures"]  ->load_signatures(get_lexicon()->get_passive_signatures());
    createTreeModel();

    print_prefix_signatures();

    delete m_graphics_scene;
    if (get_lexicon()->get_suffix_flag()) {
        m_graphics_scene = new lxa_graphics_scene(this, get_lexicon()->get_signatures());
    }else {
        m_graphics_scene = new lxa_graphics_scene(this, get_lexicon()->get_prefix_signatures());
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
    {   int max_count (10000);
        int count (0);
            QFile file(m_name_of_data_file);
            if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(m_name_of_data_file), file.errorString()));
            return;
            }

            QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
            settings.setValue("name_of_data_file", m_name_of_data_file );
            qDebug() << m_name_of_data_file << "Filename saved 2.";

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
void MainWindow::createActions()
{

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));



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
    qDebug() << m_name_of_data_file << "data file retrieved.";
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
    qDebug() << m_name_of_data_file << "Filename saved.";
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
void MainWindow::createTreeModel()
{
    QStandardItem * parent = m_treeModel->invisibleRootItem();

    QStandardItem * lexicon_item = new QStandardItem(QString("Lexicon"));
    QStandardItem * lexicon_count_item = new QStandardItem(QString("1"));

    QStandardItem * word_item = new QStandardItem(QString("Words"));
    QStandardItem * word_count_item = new QStandardItem(QString::number(get_lexicon()->get_word_collection()->get_count()));

    QStandardItem * stem_item = new QStandardItem(QString("Stems"));
    QStandardItem * stem_count_item = new QStandardItem(QString::number(get_lexicon()->GetStemCollection()->get_count()));

    QStandardItem * suffix_item = new QStandardItem(QString("Suffixes"));
    QStandardItem * suffix_count_item = new QStandardItem(QString::number(get_lexicon()->GetSuffixCollection()->get_count()));

    QStandardItem * sig_item = new QStandardItem(QString("Signatures"));
    QStandardItem * sig_count_item = new QStandardItem(QString::number(get_lexicon()->GetSignatureCollection()->get_count()));

    QStandardItem * prefix_sig_item = new QStandardItem(QString("Prefix signatures"));
    QStandardItem * prefix_sig_count_item = new QStandardItem(QString::number(get_lexicon()->get_prefix_signatures()->get_count()));

    QStandardItem * residual_sig_item = new QStandardItem(QString("Residual parasignatures"));
    QStandardItem * residual_sig_count_item = new QStandardItem(QString::number(get_lexicon()->get_residual_signatures()->get_count()));

    QStandardItem * parasuffix_item = new QStandardItem(QString("Parasuffixes"));
    QStandardItem * parasuffix_count_item = new QStandardItem(QString::number(get_lexicon()->get_parasuffixes()->get_count()));

    QStandardItem * sig_tree_edge_item = new QStandardItem(QString("Signature tree edges"));
    QStandardItem * sig_tree_edge_count_item = new QStandardItem(QString::number(get_lexicon()->get_sig_tree_edge_map()->size()));

    QStandardItem * passive_signature_item = new QStandardItem(QString("Passive signatures"));
    QStandardItem * passive_signature_count_item = new QStandardItem(QString::number(get_lexicon()->get_passive_signatures()->get_count()));



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

    QList<QStandardItem*> prefix_sig_items;
    prefix_sig_items.append(prefix_sig_item);
    prefix_sig_items.append(prefix_sig_count_item);

    QList<QStandardItem*> residual_sig_items;
    residual_sig_items.append(residual_sig_item);
    residual_sig_items.append(residual_sig_count_item);

    QList<QStandardItem*> parasuffix_items;
    parasuffix_items.append(parasuffix_item);
    parasuffix_items.append(parasuffix_count_item);

    QList<QStandardItem*> sig_tree_edge_items;
    sig_tree_edge_items.append(sig_tree_edge_item);
    sig_tree_edge_items.append(sig_tree_edge_count_item);

    QList<QStandardItem*> passive_signature_items;
    passive_signature_items.append(passive_signature_item);
    passive_signature_items.append(passive_signature_count_item);


    parent->appendRow(lexicon_items);

    lexicon_item->appendRow(word_items);
    lexicon_item->appendRow(stem_items);
    lexicon_item->appendRow(suffix_items);
    lexicon_item->appendRow(sig_items);
    lexicon_item->appendRow(prefix_sig_items);
    lexicon_item->appendRow(sig_tree_edge_items);
    lexicon_item->appendRow(residual_sig_items);
    lexicon_item->appendRow(parasuffix_items);
    lexicon_item->appendRow(passive_signature_items);

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
    if (m_tableView_upper->get_document_type()== SIGNATURES){
        if (m_tableView_upper->get_signature_sort_style()==SIG_BY_STEM_COUNT){
            m_tableView_upper->set_signature_sort_style(SIG_BY_AFFIX_COUNT);
        } else{
            m_tableView_upper->set_signature_sort_style(SIG_BY_STEM_COUNT);
        }
    }

}
