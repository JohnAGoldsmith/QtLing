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
     
public:
    MainWindow();

    void loadFile(const QString &fileName);
    void DisplaySignatures();

protected:
    void closeEvent(QCloseEvent *event) override;
//    void FindProtostems();
//    void CreateStemAffixPairs();
//    void AssignSuffixesToStems();
private slots:
    void newFile();
    void open();
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

    void loadWords();
    void loadStems();
    void loadSuffixes();
    void loadSignatures();
    void loadProtostems();

    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QSplitter *mainSplitter;
    QSplitter *rightSplitter;
    QTableView *tableView;
    QTreeView *treeView;
    QStandardItemModel *treeModel;
    QStandardItemModel *tableModel;

    QGroupBox *horizontalGroupBox;
    QGroupBox *verticalGroupBox;
    QPlainTextEdit *textEdit;
    QPlainTextEdit *littleEditor;
    QString curFile;
};

#endif
