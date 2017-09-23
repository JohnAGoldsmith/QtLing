#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QTreeView>

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


MainWindow::MainWindow()
    : tableView_upper(new QTableView),  tableView_lower(new QTableView)
{
    Lexicon = new CLexicon();
    treeModel = new QStandardItemModel();

    Word_model= new QStandardItemModel(1,3);
    Stem_model= new QStandardItemModel;
    Signature_model= new QStandardItemModel;
    Affix_model= new QStandardItemModel();




    createSplitter();
    setCentralWidget(mainSplitter);
    createActions();
    createStatusBar();

    readSettings();

//    connect(textEdit->document(), &QTextDocument::contentsChanged,
//            this, &MainWindow::documentWasModified);

#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);
}
void MainWindow::createSplitter()
{
    mainSplitter = new QSplitter();
    rightSplitter = new QSplitter(Qt::Vertical);

    rightSplitter->addWidget(tableView_upper);
    rightSplitter->addWidget(tableView_lower);
    rightSplitter->addWidget(tableView_lower);

    treeView = new QTreeView(rightSplitter);
    treeView->setModel(treeModel);

    connect(treeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(rowClicked(const QModelIndex&)));
    mainSplitter->addWidget(treeView);
    mainSplitter->addWidget(rightSplitter);
}

// not used: delete...
void MainWindow::createHorizontalGroupBox()
{
    horizontalGroupBox = new QGroupBox(tr("Horizontal layout"));
    QHBoxLayout *layout = new QHBoxLayout;

    verticalGroupBox = new QGroupBox(tr("Vertical layout"));
    QVBoxLayout *vLayout = new QVBoxLayout;

    verticalGroupBox->setLayout(vLayout);

    QTreeView* tree = new QTreeView(horizontalGroupBox);
    tree->header()->resizeSection(0, 100);
    layout->addWidget(tree);
    layout->addWidget(verticalGroupBox);
    horizontalGroupBox->setLayout(layout);
}



void MainWindow::load_word_model()
{   //CWordCollection * Words = Lexicon->GetWordCollection();
    QMap<QString,CWord*> * WordMap = Lexicon->GetWordCollection()->GetMap();
    QMapIterator<QString,CWord*> word_iter(*WordMap);
    while (word_iter.hasNext())
    {   CWord* pWord = word_iter.next().value();
        QList<QStandardItem*> item_list;

        QStandardItem* pItem = new QStandardItem(pWord->GetWord());
        item_list.append(pItem);

        QStandardItem* pItem2 = new QStandardItem(QString::number(pWord->GetWordCount()));
        item_list.append(pItem2);
        //qDebug() << pWord->GetWord() << pWord->GetWordCount();

        QListIterator<CSignature*> sig_iter(*pWord->GetSignatures());
        while (sig_iter.hasNext()){
            QStandardItem* pItem3 = new QStandardItem(sig_iter.next()->GetSignature());
            item_list.append(pItem3);
        }

        Word_model->appendRow(item_list);
    }
}
void MainWindow::load_stem_model()
{
    CStem* stem;
    QMap<QString, CStem*>::iterator iter;
    qDebug() << "Running load stems.";
    int row = 0;
    for (iter = Lexicon->GetStemCollection()->GetBegin(); iter != Lexicon->GetStemCollection()->GetEnd(); ++iter)
    {
        stem = iter.value();
        QStandardItem *item = new QStandardItem(stem->GetStem());
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
    CSuffix* suffix;
    QMapIterator<QString, CSuffix*> suffix_iter(Lexicon->GetSuffixCollection()->GetMap());

    while (suffix_iter.hasNext())
    {
        CSuffix* pSuffix = suffix_iter.next().value();
        QStandardItem *item = new QStandardItem(pSuffix->GetSuffix());
        QList<QStandardItem*> item_list;
        item_list.append(item);
        Affix_model->appendRow(item_list);

    }

}

void display_signatures()
{

}

void MainWindow::load_signature_model()
{   CSignature* sig;
    Lexicon->GetSignatures()->sort();
    QList<CSignature*>* pSortedSignatures = Lexicon->GetSignatures()->GetSortedSignatures();
    QListIterator<CSignature*> iter(*pSortedSignatures );

    while (iter.hasNext())
    {
        sig = iter.next();
        QList<QStandardItem*> items;
        QStandardItem * item2 = new QStandardItem(QString::number(sig->get_number_of_stems()));
        QStandardItem * item3 = new QStandardItem(QString::number(sig->get_robustness()));
        items.append(new QStandardItem(sig->GetSignature()));
        items.append(item2);
        items.append(item3);
        Signature_model->appendRow(items);
        //qDebug() << sig->GetSignature();

    }

}

void MainWindow::rowClicked(const QModelIndex &index)
{
    QStandardItem *item = treeModel->itemFromIndex(index);
    QString key = item->text();
    if (key == "Words"){
        //load_word_model();
        tableView_upper->setModel(Word_model);
    }
    else if (key == "Stems"){
        //load_stem_model();
        tableView_upper->setModel(Stem_model);
        qDebug() << "Clicked on show stems.";
    }
    else if (key == "Suffixes"){
        tableView_upper->setModel(Affix_model);
    }
    else if (key == "Signatures"){
        tableView_upper->setModel(Signature_model);
        qDebug() <<"clicked on signatures";
    }
    else
        qDebug() << "Invalid selection: rowClicked";
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}
void MainWindow::newFile()
{
    if (maybeSave()) {
        setCurrentFile(QString());
    }
}
void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            read_dx1_file(fileName);

        Lexicon->Crab_1();
        qDebug()<<"Finished crab.";
        load_word_model();
        load_signature_model();
        load_affix_model();
        load_stem_model();
        createTreeModel();

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
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon("../../../../QtLing/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon("../../../../QtLing/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon("../../../../QtLing/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

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
//    connect(textEdit, &QPlainTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
//    connect(textEdit, &QPlainTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
#endif // !QT_NO_CLIPBOARD
}
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}
void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
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
}
bool MainWindow::maybeSave()
{
//    if (!textEdit->document()->isModified())
//        return true;
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
    QStandardItem *parent = treeModel->invisibleRootItem();
    QStandardItem *word_item = new QStandardItem(QString("Words"));
    QStandardItem * word_count_item = new QStandardItem(QString::number(Lexicon->GetWordCollection()->get_count()));
    QStandardItem *stem_item = new QStandardItem(QString("Stems"));
    QStandardItem * stem_count_item = new QStandardItem(QString::number(Lexicon->GetStemCollection()->get_count()));
    QStandardItem *suffix_item = new QStandardItem(QString("Suffixes"));
    QStandardItem * suffix_count_item = new QStandardItem(QString::number(Lexicon->GetSuffixCollection()->get_count()));

    QStandardItem *sig_item = new QStandardItem(QString("Signatures"));
    QStandardItem * sig_count_item = new QStandardItem(QString::number(Lexicon->GetSignatureCollection()->get_count()));

    QStandardItem *prStemItem = new QStandardItem(QString("Protostems"));

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


    parent->appendRow(word_items);
    parent->appendRow(stem_items);
    parent->appendRow(suffix_items);
    parent->appendRow(sig_items);
    parent->appendRow(prStemItem);
}

void MainWindow::read_dx1_file(const QString fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    CWordCollection * Words = Lexicon->GetWordCollection();

    while (!in.atEnd())
    {
        QString line = in.readLine();
        line.simplified(); // get rid of extra spaces
        QStringList words = line.split(" ");
        QString word = words[0];
        CWord* pWord = *Words << word;
        pWord->SetWordCount(words[1].toInt());

    }
    Words->sort_word_list();

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
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
        if (!maybeSave())
            manager.cancel();
    } else {
        // Non-interactive: save without asking
//        if (textEdit->document()->isModified())
//            save();
    }
}
#endif


