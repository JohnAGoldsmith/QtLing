#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMap>
#include <QList>
#include "Lexicon.h"

QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QPlainTextEdit;
class QSessionManager;
class QSplitter;
class QStandardItemModel;
class QTextEdit;
class QTableView;
class QTreeView;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    CLexicon *Lexicon;
    QString m_name_of_data_file;
     
public:
    MainWindow();


    void DisplaySignatures();

protected:
    void closeEvent(QCloseEvent *event) override;
//    void FindProtostems();
//    void CreateStemAffixPairs();
//    void AssignSuffixesToStems();
private slots:
    void newFile();
    void read_file_do_crab();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
    void rowClicked(const QModelIndex& index);

#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager &);
#endif

private:


    void createHorizontalGroupBox();
    void createSplitter();
    void createTreeModel();
    void ask_for_filename();
    void load_word_model();
    void read_dx1_file();
    void read_text_file();
    void load_stem_model();
    void load_affix_model();
    void load_signature_model();
    void load_protostem_model();

    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool ask_to_save();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QSplitter *mainSplitter;
    QSplitter *rightSplitter;
    QTableView *tableView_upper;
    QTableView *tableView_lower;
    QTreeView *treeView;
    QStandardItemModel *treeModel;

    QStandardItemModel * Word_model;
    QStandardItemModel * Stem_model;
    QStandardItemModel * Signature_model;
    QStandardItemModel * Affix_model;
    QStandardItemModel * Protostem_model;


    QGroupBox *horizontalGroupBox;
    QGroupBox *verticalGroupBox;
    //QPlainTextEdit *textEdit;
    //QPlainTextEdit *littleEditor;
    QString curFile;
};

#endif
