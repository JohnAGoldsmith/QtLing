#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QMap>
#include <QList>
#include <QObject>
#include <QTreeView>
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
class QTreeView;
class MainWindow;
QT_END_NAMESPACE



class UpperTableView : public QTableView
{
    Q_OBJECT

    QString m_content;
    MainWindow* m_parent_window;
public:
    UpperTableView ();
    UpperTableView (MainWindow*);
    QString get_content() {return m_content;}
    void    set_content_type(QString text) {m_content = text;}
    MainWindow* get_parent_window() {return m_parent_window;}


public slots:

    void ShowModelsUpperTableView(const QModelIndex& );

    signals:
    void please_display_this_signature(QString sig);


};


class LowerTableView : public QTableView
{
    Q_OBJECT

    MainWindow* m_parent_window;
    int     m_number_of_columns;
    QStandardItemModel* m_my_current_model;

public:
    LowerTableView();
    LowerTableView(MainWindow*);
    CLexicon * p_lexicon;
    MainWindow* get_parent_window() {return m_parent_window;}
    int m_how_many_columns;
   // void set_parent(MainWindow* window) {m_parent_window = window;}
    void change_current_model(QStandardItemModel*);


public slots:



        void display_this_item(const QModelIndex&);
        int  get_number_of_columns () {return m_how_many_columns;}
signals:


};


class LeftSideTreeView : public QTreeView
{
    Q_OBJECT

    MainWindow * m_parent_window;

public:
    LeftSideTreeView(MainWindow*  window);
    //void set_parent(MainWindow* window) {m_parent_window = window;}
    void rowClicked(const QModelIndex& index);
};


class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class LeftSideTreeView;
    friend class UpperTableView;
    friend class LowerTableView;
    QList<CLexicon*>    m_lexicon_list;
    QString             m_name_of_data_file;
     

public:
    MainWindow();

    void DisplaySignatures();
        CLexicon*  get_lexicon() {return m_lexicon_list.last();  }

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
//    void rowClicked(const QModelIndex& index);

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

    QSplitter *         m_mainSplitter;
    QSplitter *         m_rightSplitter;
    UpperTableView *    m_tableView_upper;
    LowerTableView *    m_tableView_lower;
    LeftSideTreeView *  m_leftTreeView;
//    QTreeView *         treeView;

    QStandardItemModel * m_treeModel;
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
