#include "mainwindow.h"
//#include "generaldefinitions.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QTreeView>
#include <QKeyEvent>
#include "iostream"
#include <QStandardItemModel>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDebug>
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


typedef  QMap<QString,CWord*> StringToWordPtr;
typedef  QPair<CStem*,CSignature*>  stem_sig_pair;
typedef  QPair<stem_sig_pair*,  stem_sig_pair*> pair_of_stem_sig_pairs;
typedef  QPair<QString, pair_of_stem_sig_pairs*> five_tuple_sig_diffs;

LxaStandardItemModel::LxaStandardItemModel()
{

};
void LxaStandardItemModel::sort(int column_no, Qt::SortOrder order)
{
    if (column_no == 4){

    }
    else{
        QStandardItemModel::sort(column_no);
    }
}


MainWindow::MainWindow()
{

    m_lexicon_list.append ( new CLexicon() );

    // models
    m_treeModel     = new QStandardItemModel();
    Word_model      = new QStandardItemModel(1,3);
    Stem_model      = new QStandardItemModel();
    Signature_model = new QStandardItemModel();
    SingletonSignature_model = new QStandardItemModel();
    ResidualSignature_model = new QStandardItemModel();
    Affix_model     = new QStandardItemModel();
    SigTreeEdge_model = new QStandardItemModel();

    // views
    m_leftTreeView      = new LeftSideTreeView(this);
    m_tableView_upper   = new UpperTableView (this);
    m_tableView_lower   = new LowerTableView (this);
    m_tableView_upper->setSortingEnabled(true);

    // set model for tree view
    m_leftTreeView->setModel(m_treeModel);
    m_leftTreeView->setColumnWidth(1,100);
    m_leftTreeView->setColumnWidth(0,500);

    // layout
    m_mainSplitter = new QSplitter();
    m_rightSplitter = new QSplitter(Qt::Vertical);   
    m_rightSplitter->addWidget(m_tableView_upper);
    m_rightSplitter->addWidget(m_tableView_lower);
    m_mainSplitter->addWidget(m_leftTreeView);
    m_mainSplitter->addWidget(m_rightSplitter);
    setCentralWidget(m_mainSplitter);

    QProgressBar * progress = new QProgressBar(this);
    progress->setVisible(false);
    statusBar()->addPermanentWidget(progress);


    createActions();
    createStatusBar();
    readSettings();


    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    m_mainSplitter->setSizes(QList<int>() << 1000 <<4000);

    // By default, we open the last dx1 file that was opened on the previous run. This is probably not a good idea long-term!
    statusBar()->showMessage("Reading dx1 file.");
    read_dx1_file();
    statusBar()->showMessage("Finished reading dx1 file.");

#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif

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
    QMainWindow::keyPressEvent(ke);
}






///////////////////////////////////////

//         load various models

void MainWindow::load_word_model()
{

    StringToWordPtr * WordMap = get_lexicon()->GetWordCollection()->GetMap();
    QMapIterator<QString,CWord*> word_iter(*WordMap);
    while (word_iter.hasNext())
    {   CWord* pWord = word_iter.next().value();
        QList<QStandardItem*> item_list;

        QStandardItem* pItem = new QStandardItem(pWord->GetWord());
        item_list.append(pItem);

        QStandardItem* pItem2 = new QStandardItem(QString::number(pWord->GetWordCount()));
        item_list.append(pItem2);

        QListIterator<QPair<CStem*,CSignature*>*> sig_iter(*pWord->GetSignatures());
        while (sig_iter.hasNext()){
            QStandardItem* pItem3 = new QStandardItem(sig_iter.next()->second->GetSignature());
            item_list.append(pItem3);
        }

        Word_model->appendRow(item_list);
    }
}
void MainWindow::load_stem_model()
{
    CStem* stem;
    QMapIterator<QString, CStem*> * iter;
    iter = get_lexicon()->GetStemCollection()->get_map_iterator();
    while (iter->hasNext())
    {
        stem = iter->next().value();
        QStandardItem *item = new QStandardItem(stem->get_key());
        QList<QStandardItem*> item_list;
        item_list.append(item);
        QListIterator<QString> sig_iter(*stem->GetSignatures());
        while (sig_iter.hasNext()){
           QString sig = sig_iter.next();
           QStandardItem *item = new QStandardItem(sig);
           item_list.append(item);
        }
        Stem_model->appendRow(item_list);
    }
}
void MainWindow::load_affix_model()
{
     QMapIterator<QString, CSuffix*> suffix_iter(get_lexicon()->GetSuffixCollection()->GetMap());
    while (suffix_iter.hasNext())
    {
        CSuffix* pSuffix = suffix_iter.next().value();
        QStandardItem *item = new QStandardItem(pSuffix->GetSuffix());
        QStandardItem *item2 = new QStandardItem(QString::number(pSuffix->get_count()));
        QList<QStandardItem*> item_list;
        item_list.append(item);
        item_list.append(item2);
        Affix_model->appendRow(item_list);
    }
}

void MainWindow::load_signature_model()
{

    CSignature* sig;
    get_lexicon()->get_signatures()->sort();
    QListIterator<CSignature*> * iter = get_lexicon()->get_signatures()->get_sorted_list_iterator();
    while (iter->hasNext())
    {
        sig = iter->next();
        QList<QStandardItem*> items;
        QStandardItem * item2 = new QStandardItem(QString::number(sig->get_number_of_stems()));
        QStandardItem * item3 = new QStandardItem(QString::number(sig->get_robustness()));
        items.append(new QStandardItem(sig->GetSignature()));
        items.append(item2);
        items.append(item3);
        Signature_model->appendRow(items);
    }
}

struct{
    bool operator ()(sig_tree_edge* a, sig_tree_edge* b) const {
     return a->words.size() > b->words.size();
    }
}custom_compare;
void MainWindow::load_sig_tree_edge_model()
{   QList<sig_tree_edge*>               temp_list;
    QMapIterator<word_t, sig_tree_edge*> * this_sig_tree_edge_iter = new QMapIterator<word_t, sig_tree_edge*>(* get_lexicon()->get_sig_tree_edge_map());
    while (this_sig_tree_edge_iter->hasNext())    {
        this_sig_tree_edge_iter->next();
        temp_list.append(this_sig_tree_edge_iter->value());
    }
    std::sort( temp_list.begin(),  temp_list.end(), custom_compare);
    QListIterator<sig_tree_edge*> temp_list_iter (temp_list);
    while (temp_list_iter.hasNext())
     {
        sig_tree_edge * p_sig_tree_edge = temp_list_iter.next();
        QList<QStandardItem*> items;
        QStandardItem * item1 = new QStandardItem (p_sig_tree_edge->morph);
        items.append(item1);

        QStandardItem * item2 = new QStandardItem(p_sig_tree_edge->sig_1->get_key());
        QStandardItem * item3 = new QStandardItem(p_sig_tree_edge->sig_2->get_key());
        QStandardItem * item4 = new QStandardItem(QString::number(p_sig_tree_edge->words.size()));
        QStandardItem * item5 = new QStandardItem(p_sig_tree_edge->label());
        items.append(item2);
        items.append(item3);
        items.append(item4);
        items.append(item5);

        SigTreeEdge_model->appendRow(items);

    }

    qDebug() << "finished loading sig tree edge model.";
}

void MainWindow::load_singleton_signature_model()
{
    CSignature* sig;
    get_lexicon()->get_singleton_signatures()->sort();
    QListIterator<CSignature*> * iter = get_lexicon()->get_singleton_signatures()->get_sorted_list_iterator();
    while (iter->hasNext())
    {
        sig = iter->next();
        QList<QStandardItem*> items;
        QStandardItem * item2 = new QStandardItem(QString::number(sig->get_number_of_stems()));
        QStandardItem * item3 = new QStandardItem(QString::number(sig->get_robustness()));
        items.append(new QStandardItem(sig->GetSignature()));
        items.append(item2);
        items.append(item3);
        SingletonSignature_model->appendRow(items);
    }
}

// Signatures with only one stem.
void MainWindow::load_residual_signature_model()
{
    QList<QStandardItem*> items;
    int count = 0;
    CSignature* sig;
    QString affix;
    int max_stem_count = 20;

    get_lexicon()->get_residual_signatures()->sort();
    QListIterator<CSignature*> * iter =get_lexicon()->get_residual_signatures()->get_sorted_list_iterator() ;

    while (iter->hasNext())
    {   count++;
        if (count > 10000) {break;}
        sig = iter->next();
        if (sig->get_number_of_stems() > max_stem_count) {
            continue;
        }
        QList<QStandardItem*> items;
        QStandardItem * stem_item = new QStandardItem(sig->get_stems()->first()->get_key());
        items.append(stem_item);
        QStringList affixes = sig->display().split("=");
        foreach (affix, affixes){
            QStandardItem * item2 = new QStandardItem(affix);
            items.append(item2);
        }
        ResidualSignature_model->appendRow(items);
    }
}






void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();

/*    if (ask_to_save()) {
        writeSettings();
        event->accept();
        qDebug() << " closing 1";
    } else {
        event->ignore();
        qDebug() << "   closing 2" ;
    }
*/
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
}

void MainWindow::do_crab()
{   statusBar()->showMessage("Entering the Crab Nebula.");
    get_lexicon()->Crab_1();
    statusBar()->showMessage("We have returned from the Crab Nebular.");
    load_word_model();
    load_signature_model();
    load_affix_model();
    load_stem_model();
    load_sig_tree_edge_model();
    statusBar()->showMessage("Loading the residual signatures.");
    load_residual_signature_model();
    statusBar()->showMessage("Loading the singleton siganatures.");
    load_singleton_signature_model();
    createTreeModel();
    m_leftTreeView->expandAll();
    statusBar()->showMessage("All models are loaded.");

}

void MainWindow::read_file_do_crab()
{
        read_dx1_file();
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
            qDebug() << m_name_of_data_file << "Filename saved 2.";

            QTextStream in(&file);
            CWordCollection * Words = get_lexicon()->GetWordCollection();

    while (!in.atEnd())
    {
            QString line = in.readLine();
            line = line.simplified(); // get rid of extra spaces
            QStringList words = line.split(" ");
            QString word = words[0];
            CWord* pWord = *Words << word;
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
            tr("The <b>Application</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}
void MainWindow::documentWasModified()
{
    //setWindowModified(textEdit->document()->isModified());
}
void MainWindow::createActions()
{

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));

    /*
    // No action associated with this.
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon("../../../../QtLing/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);
*/


    // Give a data file name, store the name, and read the file.
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon("../../../../QtLing/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::ask_for_filename);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);
/*
    // No action associated with this yet.
    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon("../../../../QtLing/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);
*/

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
#ifndef QT_NO_CLIPBOARD
    cutAct->setEnabled(false);

    copyAct->setEnabled(false);

#endif // !QT_NO_CLIPBOARD
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
    QStandardItem * word_count_item = new QStandardItem(QString::number(get_lexicon()->GetWordCollection()->get_count()));

    QStandardItem * stem_item = new QStandardItem(QString("Stems"));
    QStandardItem * stem_count_item = new QStandardItem(QString::number(get_lexicon()->GetStemCollection()->get_count()));

    QStandardItem * suffix_item = new QStandardItem(QString("Suffixes"));
    QStandardItem * suffix_count_item = new QStandardItem(QString::number(get_lexicon()->GetSuffixCollection()->get_count()));

    QStandardItem * sig_item = new QStandardItem(QString("Signatures"));
    QStandardItem * sig_count_item = new QStandardItem(QString::number(get_lexicon()->GetSignatureCollection()->get_count()));

    QStandardItem * residual_sig_item = new QStandardItem(QString("Residual parasignatures"));
    QStandardItem * residual_sig_count_item = new QStandardItem(QString::number(get_lexicon()->get_residual_signatures()->get_count()));

    QStandardItem * singleton_sig_item = new QStandardItem(QString("Singleton signatures"));
    QStandardItem * singleton_sig_count_item = new QStandardItem(QString::number(get_lexicon()->get_singleton_signatures()->get_count()));

    //QStandardItem * proto_stem_item = new QStandardItem(QString("Protostems"));
    //QStandardItem * proto_stem_count_item = new QStandardItem(QString::number(get_lexicon()->get_protostems()->size() ));

    QStandardItem * sig_tree_edge_item = new QStandardItem(QString("Signature tree edges"));
    QStandardItem * sig_tree_edge_count_item = new QStandardItem(QString::number(get_lexicon()->get_sig_tree_edge_map()->size()));


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

    QList<QStandardItem*> singleton_sig_items;
    singleton_sig_items.append(singleton_sig_item);
    singleton_sig_items.append(singleton_sig_count_item);


    QList<QStandardItem*> residual_sig_items;
    residual_sig_items.append(residual_sig_item);
    residual_sig_items.append(residual_sig_count_item);


    QList<QStandardItem*> sig_tree_edge_items;
    sig_tree_edge_items.append(sig_tree_edge_item);
    sig_tree_edge_items.append(sig_tree_edge_count_item);

    parent->appendRow(lexicon_items);

    lexicon_item->appendRow(word_items);
    lexicon_item->appendRow(stem_items);
    lexicon_item->appendRow(suffix_items);
    lexicon_item->appendRow(sig_items);
    lexicon_item->appendRow(sig_tree_edge_items);
    lexicon_item->appendRow(singleton_sig_items);
    lexicon_item->appendRow(residual_sig_items);


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
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    //out << textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

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

LowerTableView::LowerTableView()
{
   m_my_current_model =new  QStandardItemModel();
   m_number_of_columns = 6;

}

LowerTableView::LowerTableView(MainWindow * window)
{
   m_my_current_model =new  QStandardItemModel();
   m_parent_window = window;
   m_number_of_columns = 6;
}


 void LowerTableView::display_this_item( const  QModelIndex & index )
 {
     eDocumentType UpperView_type = m_parent_window->m_tableView_upper->get_document_type();
     QString component = m_parent_window->m_tableView_upper->get_content();
     QString word, stem, prefix, suffix, signature;
     CLexicon * this_lexicon = get_parent_window()->get_lexicon();
     int row;
     qDebug() << "display this item";
//  ---------------------------------------------------//
     if (UpperView_type == WORDS){
//  ---------------------------------------------------//
         if (index.isValid()){
             word = index.data().toString();
         }
        QList<QStandardItem*> item_list;
        delete m_my_current_model;
        m_my_current_model = new QStandardItemModel();
    }


     //  ---------------------------------------------------//
     else if (UpperView_type == SIGNATURES){
     //  ---------------------------------------------------//

         qDebug() << "line 555 show this signature";
         if (index.isValid()){
             signature = index.data().toString();
             qDebug() << signature;
         }

        CSignature*           pSig = this_lexicon->get_signatures()->get_signature(signature);
        CStem*                p_Stem;
        QList<CStem*>*        sig_stems = pSig->get_stems();
        QStandardItem*        p_item;
        QList<QStandardItem*> item_list;

        if (m_my_current_model) {
            delete m_my_current_model;
        }
        m_my_current_model = new QStandardItemModel();
         qDebug() << "line 569";
        foreach (p_Stem, *sig_stems)  {
            p_item = new QStandardItem(p_Stem->get_key() );
            item_list.append(p_item);

            if (item_list.length() >= m_number_of_columns){
                m_my_current_model->appendRow(item_list);
                item_list.clear();
            }
        }
        if (item_list.size() > 0){
            m_my_current_model->appendRow(item_list);
        }
                 qDebug() << "line 579";
        setModel( m_my_current_model);
    }
     //  ---------------------------------------------------//
     else if (UpperView_type == RESIDUAL_PARASIGNATURES){
     //  ---------------------------------------------------//
        if (index.isValid()){
             row = index.row();
        }
        CSignature*           pSig = this_lexicon->get_signatures()->get_signature(signature);
        CStem*                p_Stem;
        QList<CStem*>*        sig_stems = pSig->get_stems();
        QStandardItem*        p_item;
        QList<QStandardItem*> item_list;


        if (m_my_current_model) {
            delete m_my_current_model;
        }
        qDebug() << "line 568";
        m_my_current_model = new QStandardItemModel();
         qDebug() << "line 569";
        foreach (p_Stem, *sig_stems)  {
            p_item = new QStandardItem(p_Stem->get_key() );
            item_list.append(p_item);
            if (item_list.length() >= m_number_of_columns){
                m_my_current_model->appendRow(item_list);
                item_list.clear();
            }
        }
        qDebug() << "line 579";
        setModel( m_my_current_model);
    }
     //  ---------------------------------------------------//
     else if (UpperView_type == SINGLETON_SIGNATURES){
     //  ---------------------------------------------------//
        if (index.isValid()){
             row = index.row();
        }
        CSignature*           pSig = this_lexicon->get_signatures()->get_signature(signature);
        CStem*                p_Stem;
        QList<CStem*>*        sig_stems = pSig->get_stems();
        QStandardItem*        p_item;
        QList<QStandardItem*> item_list;

        qDebug() << "line 566; signature: " << signature;
        if (m_my_current_model) {
            delete m_my_current_model;
        }
        qDebug() << "line 568";
        m_my_current_model = new QStandardItemModel();
         qDebug() << "line 569";
        foreach (p_Stem, *sig_stems)  {
            p_item = new QStandardItem(p_Stem->get_key() );
            item_list.append(p_item);
            if (item_list.length() >= m_number_of_columns){
                m_my_current_model->appendRow(item_list);
                item_list.clear();
            }
        }
        qDebug() << "line 579";
        setModel( m_my_current_model);
    }
     //  ---------------------------------------------------//
     else if (UpperView_type == SIGNATURE_TREE_EDGES){
     //  ---------------------------------------------------//

     QList<QString> * word_list;
     if (index.isValid()){
         row = index.row();
     } else{return;}

     QString label = index.sibling(row,4).data().toString();
     sig_tree_edge * p_sig_tree_edge =this_lexicon->get_sig_tree_edge_map()->value(label);
     word_list = & p_sig_tree_edge->words;

     QList<QStandardItem*> item_list;
     if (m_my_current_model) {
         delete m_my_current_model;
     }
     m_my_current_model = new QStandardItemModel();
     foreach (word, *word_list)  {
         QStandardItem* p_item = new QStandardItem(word);
         item_list.append(p_item);
         if (item_list.length() >= m_number_of_columns){
             m_my_current_model->appendRow(item_list);
             item_list.clear();
         }
     }
     if (item_list.size() > 0){
        m_my_current_model->appendRow(item_list);
     }
     qDebug() << "line 579";
     setModel( m_my_current_model);
    }




   resizeColumnsToContents();

 }

 LeftSideTreeView::LeftSideTreeView(MainWindow* window)
 {
     m_parent_window = window;



 }

UpperTableView::UpperTableView (MainWindow* window)
{
        m_parent_window = window;

}
void UpperTableView::ShowModelsUpperTableView(const QModelIndex& index)
{
    QString component;
    qDebug() << "show model upper table" << index.data().toString();
    if (index.isValid()){
        component = index.data().toString();
    }
    if (component == "Words"){
        setModel(m_parent_window->Word_model);
        set_document_type( WORDS );
        set_content_type( "words");
    }
    else     if (component == "Stems"){
        setModel(m_parent_window->Stem_model);
        set_document_type( STEMS );
        set_content_type( "stems");
    }
    else     if (component == "Suffixes"){
        setModel(m_parent_window->Affix_model);
        set_document_type( SUFFIXES );
        set_content_type( "suffixes");
    }
    else     if (component == "Signatures"){
        setModel(m_parent_window->Signature_model);
        set_document_type( SIGNATURES );
        set_content_type( "signatures");
    }
    else     if (component == "Signature tree edges"){
        setModel(m_parent_window->SigTreeEdge_model);
        set_document_type( SIGNATURE_TREE_EDGES );
        set_content_type( "sigtreeedges");
        qDebug() << "line 876 we got here";
        sortByColumn(-1);
    }
    else     if (component == "Residual parasignatures"){
        setModel(m_parent_window->ResidualSignature_model);
        set_document_type( SIGNATURE_RESIDUES );
        //set_content_type( "rawsignatures");
        sortByColumn(1);
    }
    else     if (component == "Singleton signatures"){
        setModel(m_parent_window->SingletonSignature_model);
        set_document_type( SINGLETON_SIGNATURES );
        //set_content_type( "rawsignatures");
        sortByColumn(1);
    }
    resizeColumnsToContents();
}
