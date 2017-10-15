#include "mainwindow.h"
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

#include <QDir>

#include <QSplitter>
#include <QTableView>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMapIterator>

typedef  QMap<QString,CWord*> StringToWordPtr;
typedef  QPair<CStem*,CSignature*>  stem_sig_pair;
typedef  QPair<stem_sig_pair*,  stem_sig_pair*> pair_of_stem_sig_pairs;
typedef  QPair<QString, pair_of_stem_sig_pairs*> five_tuple_sig_diffs;



MainWindow::MainWindow()
{

    m_lexicon_list.append ( new CLexicon() );

    // models
    m_treeModel     = new QStandardItemModel();
    Word_model      = new QStandardItemModel(1,3);
    Stem_model      = new QStandardItemModel();
    Signature_model = new QStandardItemModel();
    Raw_Signature_model = new QStandardItemModel();
    Affix_model     = new QStandardItemModel();
    Multiparse_model = new QStandardItemModel();
    qDebug() << "reach 1";

    // views
    m_leftTreeView      = new LeftSideTreeView(this);
    m_tableView_upper   = new UpperTableView (this);
    m_tableView_lower   = new LowerTableView (this);
    m_tableView_upper->setSortingEnabled(true);


    // set model for tree view
    m_leftTreeView->setModel(m_treeModel);

    // layout
    m_mainSplitter = new QSplitter();
    m_rightSplitter = new QSplitter(Qt::Vertical);   
    m_rightSplitter->addWidget(m_tableView_upper);
    m_rightSplitter->addWidget(m_tableView_lower);
    m_mainSplitter->addWidget(m_leftTreeView);
    m_mainSplitter->addWidget(m_rightSplitter);
    setCentralWidget(m_mainSplitter);

    createActions();
    createStatusBar();
    readSettings();
    qDebug() << "reach 6";

    // By default, we open the last dx1 file that was opened on the previous run. This is probably not a good idea long-term!
    read_dx1_file();


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
        QList<QStandardItem*> item_list;
        item_list.append(item);
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
void MainWindow::load_multiparse_model()
{

    CSignature* sig;
    int multi_num;
    for (multi_num = 0; multi_num < get_lexicon()->get_multiparses()->size(); multi_num++ )
    {
        five_tuple_sig_diffs * this_five_tuple  = get_lexicon()->get_multiparses()->at(multi_num);
        QList<QStandardItem*> items;


        QString difference = this_five_tuple->first;

        QStandardItem * item1 = new QStandardItem (difference);
        items.append(item1);

        pair_of_stem_sig_pairs * this_pair_of_pairs = this_five_tuple->second;
        stem_sig_pair * stem_sig_pair_1 = this_pair_of_pairs->first;
        stem_sig_pair * stem_sig_pair_2 = this_pair_of_pairs->second;

        QStandardItem * item2 = new QStandardItem(stem_sig_pair_1->first->get_key());
        QStandardItem * item3 = new QStandardItem(stem_sig_pair_1->second->get_key());
        QStandardItem * item4 = new QStandardItem(stem_sig_pair_2->first->get_key());
        QStandardItem * item5 = new QStandardItem(stem_sig_pair_2->second->get_key());
        items.append(item2);
        items.append(item3);
        items.append(item4);
        items.append(item5);

        Multiparse_model->appendRow(items);

    }
    qDebug() << "finished loading multiparse model.";
}
void MainWindow::load_multiparse_edge_model()
{

    CSignature* sig;
    int edge_num;
    for (edge_num = 0; edge_num < get_lexicon()->get_multiparses()->size(); edge_num++ )
    {
        five_tuple_sig_diffs * this_five_tuple  = get_lexicon()->get_multiparses()->at(edge_num);
        QList<QStandardItem*> items;


        QString difference = this_five_tuple->first;

        QStandardItem * item1 = new QStandardItem (difference);
        items.append(item1);

        pair_of_stem_sig_pairs * this_pair_of_pairs = this_five_tuple->second;
        stem_sig_pair * stem_sig_pair_1 = this_pair_of_pairs->first;
        stem_sig_pair * stem_sig_pair_2 = this_pair_of_pairs->second;

        QStandardItem * item2 = new QStandardItem(stem_sig_pair_1->first->get_key());
        QStandardItem * item3 = new QStandardItem(stem_sig_pair_1->second->get_key());
        QStandardItem * item4 = new QStandardItem(stem_sig_pair_2->first->get_key());
        QStandardItem * item5 = new QStandardItem(stem_sig_pair_2->second->get_key());
        items.append(item2);
        items.append(item3);
        items.append(item4);
        items.append(item5);

        Multiparse_model->appendRow(items);

    }
    qDebug() << "finished loading multiparse model.";
}


void MainWindow::load_raw_signature_model()
{
    QList<QStandardItem*> items;
    int count = 0;
    CSignature* sig;
    QString affix;
    int max_stem_count = 20;

    get_lexicon()->get_raw_signatures()->sort();
    QListIterator<CSignature*> * iter =get_lexicon()->get_raw_signatures()->get_sorted_list_iterator() ;

    while (iter->hasNext())
    {   count++;
        if (count > 10000) {break;}
        sig = iter->next();
        if (sig->get_number_of_stems() > max_stem_count) {
            continue;
        }
        //qDebug() << sig->display();
        QList<QStandardItem*> items;
        QStringList affixes = sig->display().split("=");
        foreach (affix, affixes){
            QStandardItem * item2 = new QStandardItem(affix);
            items.append(item2);
        }


        //items.append(item2);
        //items.append(item3);
        Raw_Signature_model->appendRow(items);
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
{
    get_lexicon()->Crab_1();
    load_word_model();
    load_signature_model();
    load_affix_model();
    load_stem_model();

    load_multiparse_model();

    load_raw_signature_model();
    createTreeModel();

}

void MainWindow::read_file_do_crab()
{
        read_dx1_file();
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
            pWord->SetWordCount(words[1].toInt());
    }
    Words->sort_word_list();
    setCurrentFile(m_name_of_data_file);
    statusBar()->showMessage(tr("File loaded"), 2000);
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

    QStandardItem * word_item = new QStandardItem(QString("Words"));
    QStandardItem * word_count_item = new QStandardItem(QString::number(get_lexicon()->GetWordCollection()->get_count()));

    QStandardItem * stem_item = new QStandardItem(QString("Stems"));
    QStandardItem * stem_count_item = new QStandardItem(QString::number(get_lexicon()->GetStemCollection()->get_count()));

    QStandardItem * suffix_item = new QStandardItem(QString("Suffixes"));
    QStandardItem * suffix_count_item = new QStandardItem(QString::number(get_lexicon()->GetSuffixCollection()->get_count()));

    QStandardItem * sig_item = new QStandardItem(QString("Signatures"));
    QStandardItem * sig_count_item = new QStandardItem(QString::number(get_lexicon()->GetSignatureCollection()->get_count()));

    QStandardItem * raw_sig_item = new QStandardItem(QString("Raw signatures"));
    QStandardItem * raw_sig_count_item = new QStandardItem(QString::number(get_lexicon()->get_raw_signatures()->get_count()));

    QStandardItem * proto_stem_item = new QStandardItem(QString("Protostems"));
    QStandardItem * proto_stem_count_item = new QStandardItem(QString::number(get_lexicon()->get_protostems()->size() ));

    QStandardItem * multiparse_item = new QStandardItem(QString("Multiparses"));
    QStandardItem * multiparse_count_item = new QStandardItem(QString::number(get_lexicon()->get_multiparses()->size()));

    QStandardItem * multiparse_edge_item = new QStandardItem(QString("Multiparse edges"));
    QStandardItem * multiparse_edge_count_item = new QStandardItem(QString::number(get_lexicon()->get_multiparse_edges()->size()));

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

    QList<QStandardItem*> raw_sig_items;
    raw_sig_items.append(raw_sig_item);
    raw_sig_items.append(raw_sig_count_item);


    QList<QStandardItem*> multiparse_items;
    multiparse_items.append(multiparse_item);
    multiparse_items.append(multiparse_count_item);

    QList<QStandardItem*> multiparse_edge_items;
    multiparse_edge_items.append(multiparse_edge_item);
    multiparse_edge_items.append(multiparse_edge_count_item);

    parent->appendRow(word_items);
    parent->appendRow(stem_items);
    parent->appendRow(suffix_items);
    parent->appendRow(sig_items);
    parent->appendRow(multiparse_items);
    parent->appendRow(multiparse_edge_items);
    parent->appendRow(raw_sig_items);

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
     QString component = m_parent_window->m_tableView_upper->get_content();
     QString word, stem, prefix, suffix, signature;

     qDebug() << "display this item";

     if (component == "words"){
         if (index.isValid()){
             word = index.data().toString();
         }

        CWord*           pWord = m_parent_window->get_lexicon()->get_words()->get_word(word);
        CStem*                p_Stem;

        QStandardItem*        p_item;
        QList<QStandardItem*> item_list;

        delete m_my_current_model;
        m_my_current_model = new QStandardItemModel();


        // fsetModel( m_my_current_model);
    }



     else if (component == "signatures"){

         qDebug() << "line 555 show this signature";
         if (index.isValid()){
             signature = index.data().toString();
             qDebug() << signature;
         }

        CSignature*           pSig = m_parent_window->get_lexicon()->get_signatures()->get_signature(signature);
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
                 qDebug() << "line 579";
        setModel( m_my_current_model);
    }

     else if (component == "Raw Signatures"){

         qDebug() << "line 711";
         if (index.isValid()){
             signature = index.data().toString();
         }

        CSignature*           pSig = m_parent_window->get_lexicon()->get_signatures()->get_signature(signature);
        CStem*                p_Stem;
        QList<CStem*>*        sig_stems = pSig->get_stems();
        QStandardItem*        p_item;
        QList<QStandardItem*> item_list;

                 qDebug() << "line 566";
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
     else if (component == "multiparses"){

         qDebug() << "line 641";
         if (index.isValid()){
             signature = index.data().toString();
         }

        CSignature*           pSig = m_parent_window->get_lexicon()->get_signatures()->get_signature(signature);
        CStem*                p_Stem;
        QList<CStem*>*        sig_stems = pSig->get_stems();
        QStandardItem*        p_item;
        QList<QStandardItem*> item_list;

        qDebug() << sig_stems->size(); //at(0)->get_stem();
                 qDebug() << "line 566";
        if (m_my_current_model) {
            delete m_my_current_model;
        }
                          qDebug() << "line 568";
        m_my_current_model = new QStandardItemModel();
         qDebug() << "line 569";
        foreach (p_Stem, *sig_stems)  {
            p_item = new QStandardItem(p_Stem->get_key() );
            //qDebug() << p_Stem->get_key();
            item_list.append(p_item);

            if (item_list.length() >= m_number_of_columns){
                m_my_current_model->appendRow(item_list);
                item_list.clear();
            }
        }
                 qDebug() << "line 579";
        setModel( m_my_current_model);
    }






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
        set_content_type( "words");
    }
    else     if (component == "Stems"){
        setModel(m_parent_window->Stem_model);
        set_content_type( "stems");
    }
    else     if (component == "Suffixes"){
        setModel(m_parent_window->Affix_model);
        set_content_type( "suffixes");
    }
    else     if (component == "Signatures"){
        setModel(m_parent_window->Signature_model);
        set_content_type( "signatures");
    }
    else     if (component == "Multiparses"){
        setModel(m_parent_window->Multiparse_model);
        set_content_type( "multiparses");
        sortByColumn(1);
    }
    else     if (component == "Raw signatures"){
        setModel(m_parent_window->Raw_Signature_model);
        set_content_type( "rawsignatures");
        sortByColumn(1);
    }
}
