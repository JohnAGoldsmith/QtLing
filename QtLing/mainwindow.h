#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QMap>
#include <QList>
#include <QObject>
#include <QTreeView>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QListView>
#include <QGraphicsView>
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
class lxa_graphics_view;
class lxa_graphics_scene;
QT_END_NAMESPACE

class lxaWindow2 : public QGraphicsView
{
    QList<QList<CSignature*>*>  m_signature_lattice;
    MainWindow *    m_main_window;
    QGraphicsScene  m_graphics_scene;

public:
    lxaWindow2( MainWindow * );
};

class lxaWindow : public QWidget
{   QList<QList<CSignature*>*>  m_signature_lattice;
    MainWindow *    m_main_window;
    double          m_scale;
    double          m_xshift;
    double          m_yshift;

public:
    lxaWindow( MainWindow * );
    virtual void    paintEvent(QPaintEvent * );
    void            drawSignatures(QPainter& painter);
    void            ingest_signatures(CSignatureCollection* );
    void            expand();
    void            contract();
    void            move_down();
    void            move_up();
    void            reset_scale_and_translation();
};

class LxaStandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    LxaStandardItemModel();
    LxaStandardItemModel(QString shortname);
    ~LxaStandardItemModel();

private:
    QString                 m_ShortName;
    QString                 m_Description;

public:

    //QSortFilterProxyModel*          get_proxy() {return m_proxy;}
    void        sort(int column_no, Qt::SortOrder order);
    void        load_words(CWordCollection*);
    void        load_stems(CStemCollection * p_stems);
    void        load_suffixes(CSuffixCollection * p_suffixes);
    void        load_signatures(CSignatureCollection * p_signatures);
    void        load_sig_tree_edges(QMap<QString, sig_tree_edge*> *);
};


class UpperTableView : public QTableView
{
    Q_OBJECT
    QString         m_content;
    MainWindow*     m_parent_window;
    eDocumentType   m_document_type;
public:
    UpperTableView ();
    UpperTableView (MainWindow*);
    QString         get_content()                   {return m_content;}
    void            set_content_type(QString text)  {m_content = text;}
    MainWindow*     get_parent_window()             {return m_parent_window;}
    eDocumentType   get_document_type()             {return m_document_type;}
    void            set_document_type(eDocumentType type) {m_document_type = type;}
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
    void change_current_model(QStandardItemModel*);
public slots:
        void display_this_item(const QModelIndex&);
        int  get_number_of_columns () {return m_number_of_columns;}

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
    friend class        LeftSideTreeView;
    friend class        UpperTableView;
    friend class        LowerTableView;
    friend class        lxaWindow;
    QList<CLexicon*>    m_lexicon_list;
    QString             m_name_of_data_file;
    QProgressBar *      m_ProgressBar;
    QMap<QString, LxaStandardItemModel*>   m_Models;

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
    void do_crab2();
    void do_crab();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
    void print_prefix_signatures();
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
    void load_multiparse_model();
    void load_raw_signature_model();
    void load_sig_tree_edge_model();
    void load_residual_signature_model();
    void load_singleton_signature_model();
    void load_singleton_stem_model();
    void load_StemSet2_model();
    void load_subsignature_model();

    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool ask_to_save();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    lxa_graphics_view*      get_graphics_view() {return  m_graphics_view;}
    lxa_graphics_view*      get_graphics_view_2() {return  m_graphics_view_2;}
    lxa_graphics_scene*     get_graphics_scene_1()  {return m_graphics_scene_1;}
    lxa_graphics_scene*     get_graphics_scene_2() {return m_graphics_scene_2;}
    QSplitter *             m_mainSplitter;
    QSplitter *             m_rightSplitter;
    UpperTableView *        m_tableView_upper;
    LowerTableView *        m_tableView_lower;
    LeftSideTreeView *      m_leftTreeView;
    lxa_graphics_scene *    m_graphics_scene_1;
    lxa_graphics_scene *    m_graphics_scene_2;
    lxa_graphics_scene *    m_current_graphics_scene ;
    lxa_graphics_view *     m_graphics_view;
    lxa_graphics_view *     m_graphics_view_2;
    bool                    m_graphic_display_flag;
    QStandardItemModel *    m_treeModel;


    QGroupBox *horizontalGroupBox;
    QGroupBox *verticalGroupBox;
    QString curFile;

protected:
   void keyPressEvent(QKeyEvent *e);

};



#endif
