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
#include "table_views.h"


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
class LxaStandardItemModel;
class UpperTableView;
class LowerTableView;
class LeftSideTreeView;

QT_END_NAMESPACE

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
    void            assign_scene_positions_to_signatures(CSignatureCollection* );
    void            expand();
    void            contract();
    void            move_down();
    void            move_up();
    void            reset_scale_and_translation();
};




//-----------------------------------------------------------------//
class MainWindow : public QMainWindow
//-----------------------------------------------------------------//
{
    Q_OBJECT
    friend class                            LeftSideTreeView;
    friend class                            UpperTableView;
    friend class                            LowerTableView;
    friend class                            lxaWindow;

    QList<CLexicon*>                        m_lexicon_list;
    QString                                 m_name_of_data_file;
    QProgressBar *                          m_ProgressBar;
    QMap<QString, LxaStandardItemModel*>    m_Models;
    eDataType                               m_graphic_display_type;
    eDataType                               m_upper_table_left_data_type;
    eDataType                               m_upper_table_right_data_type;
    QSplitter *                             m_mainSplitter;
    QSplitter *                             m_rightSplitter;
    QSplitter *                             m_top_rightSplitter;
    UpperTableView *                        m_tableView_upper_left;
    UpperTableView *                        m_tableView_upper_right;
    LowerTableView *                        m_tableView_lower;
    LeftSideTreeView *                      m_leftTreeView;
    lxa_graphics_scene *                    m_current_graphics_scene;
    lxa_graphics_scene *                    m_suffix_graphics_scene;
    lxa_graphics_scene *                    m_prefix_graphics_scene;
    lxa_graphics_view *                     m_graphics_view;
    bool                                    m_graphic_display_flag;
    QStandardItemModel *                    m_treeModel;

    QGroupBox *                             horizontalGroupBox;
    QGroupBox *                             verticalGroupBox;
    QString                                 curFile;



public:
    MainWindow();
    void        display_suffixes();
    void        display_suffix_signatures();
    void        display_prefix_signatures();
    void        display_epositive_suffix_signatures();
    void        display_hypotheses();
    void        DisplaySignatures();
    CLexicon*   get_lexicon() {return m_lexicon_list.last();  }

protected:
    void closeEvent(QCloseEvent *event) override;

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

#ifndef QT_NO_SESSIONMANAGER
    void                    commitData(QSessionManager &);
#endif

public:

    void createHorizontalGroupBox();
    void createSplitter();
    void ask_for_filename();
    void read_dx1_file();
    void read_text_file();

//          Qt-style modesl
    void                    load_stem_model();
    void                    load_affix_model();
    void                    load_signature_model();
    void                    load_protostem_model();
    void                    load_multiparse_model();
    void                    load_raw_signature_model();
    void                    load_sig_tree_edge_model();
    void                    load_residual_signature_model();
    void                    load_singleton_signature_model();
    void                    load_singleton_stem_model();
    void                    load_StemSet2_model();
    void                    load_subsignature_model();
    void                    load_word_model();
    void                    create_or_update_TreeModel();

    void                    sort_upper_table();

//          windows
    void                    createActions();
    void                    createStatusBar();

//          settings
    void                    readSettings();
    void                    writeSettings();

//          working with files
    bool                    ask_to_save();
    bool                    saveFile(const QString &fileName);
    void                    setCurrentFile(const QString &fileName);
    QString                 strippedName(const QString &fullFileName);
    void                    cycle_through_graphic_displays();

    lxa_graphics_view*      get_graphics_view()     {return  m_graphics_view;}
    lxa_graphics_scene*     get_graphics_scene()    {return m_current_graphics_scene;}
    lxa_graphics_scene*     get_suffix_graphics_scene()     {return m_suffix_graphics_scene;}
    lxa_graphics_scene*     get_prefix_graphics_scene()    {return m_prefix_graphics_scene;}
    UpperTableView *        get_upper_right_tableview() { return m_tableView_upper_right;}
    void                    set_current_graphics_scene(lxa_graphics_scene* this_scene) {m_current_graphics_scene = this_scene;}

protected:
   void keyPressEvent(QKeyEvent *e);

};



#endif
