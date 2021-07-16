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
#include "table_views.h"
#include <QProgressBar>
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
class LxaStandardItemModel;
class LxaSortFilterProxyModel;
class UpperTableView;
class LowerTableView;
class LeftSideTreeView;
class MainMenuBar;
class FindDockWidget;

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
    friend class                            CLexicon;
    friend class                            MainMenu;
    friend class                            MainMenuBar;

    QStringList m_model_names {
        "Words",
        "Suffixal stems",
        "Prefixal stems",
        "Suffixes",
        "Prefixes",
        "Signatures",
        "EPositive signatures",
        "Prefix signatures",
        "EPositive prefix signatures",
        "Residual parasignatures",
        "SigGraphEdges_1",
        "SigGraphEdges_2",
        "Parasuffixes",
        "Passive signatures",
        "Hypotheses",
        "Hypotheses 2",
        "Suffixal protostems",
        "Prefixal protostems",
        "Compound words",
        "Parses"
    };
    QStringList m_duplicate_model_names {
        "Words 2",
        "Prefixes 2",
        "Suffixes 2",
        "Signatures 2",
        "Prefix signatures 2",
        "EPositive signatures 2",
        "EPositive prefix signatures 2"
    };

    QList<CLexicon*>                        m_lexicon_list;
    CLexicon*                               m_my_lexicon;
    QStringList                             m_corpus;
    QString                                 m_name_of_data_file;
    QString                                 m_name_of_project_file;
    QProgressBar *                          m_ProgressBar;
    QStatusBar *                            m_status_bar;
    QMap<QString, LxaStandardItemModel*>    m_Models;
    QMap<QString, LxaSortFilterProxyModel*> m_proxy_models;
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

    lxa_graphics_scene *                    m_graphics_scene;
    lxa_graphics_view *                     m_graphics_view;
    bool                                    m_graphic_display_flag;
    QStandardItemModel *                    m_treeModel;

    QGroupBox *                             horizontalGroupBox;
    QGroupBox *                             verticalGroupBox;
    QString                                 curFile;
    FindDockWidget *                        m_find_dock_widget;

    // ACTIONS, see mainwindow_actions.cpp
    MainMenuBar *                           m_main_menu_bar;
    QAction *                               openAct;
    QAction *                               saveAsAct;
    QAction *                               exitAct;

    QAction *                               cutAct;
    QAction *                               copyAct;
    QAction *                               pasteAct;
    QAction *                               findAct;
    QAction *                               findLeftAct;
    QAction *                               findRightAct;

    QAction *                               aboutAct;
    QAction *                               aboutQtAct;

    QAction *                               importAct;
    QAction *                               evalAct;
    QAction *                               importMorfessorAct;
    QAction *                               evalMorfessorAct;

    QAction *                               crab_suffix_1_Act;
    QAction *                               crab_prefix_1_Act;
    QAction *                               crab_suffix_2_Act;
    QAction *                               crab_prefix_2_Act;


public:
    void                                    analyze_corpus();
    MainWindow();
    void                                    display_words();
    void                                    display_suffixes();
    void                                    display_prefixes();
    void                                    display_suffix_signatures(CLexicon*);
    void                                    display_prefix_signatures(CLexicon*);
    void                                    display_epositive_suffix_signatures(CLexicon*);
    void                                    display_epositive_prefix_signatures(CLexicon*);
    void                                    display_signature_graph_edges(CLexicon* );
    void                                    display_hypotheses();
//    void                                    DisplaySignatures();
    CLexicon*                               get_lexicon()                       { return m_my_lexicon;  }
    QList<CLexicon*>*                       get_lexica()                        { return& m_lexicon_list; }
    bool                                    get_graphic_display_flag()          { return m_graphic_display_flag; }
    void                                    load_models(CLexicon* lexicon);
    void                                    read_corpus();
   // void                                    set_up_graphics_scene_and_view();

protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void                                    xml_parsed();
    void                                    lexicon_ready();
    void                                    morfessor_parsed();

private slots:
    void                                    about();

    void                                    do_crab1_suffixes();
    void                                    do_crab1_prefixes();
    void                                    do_crab1();
    void                                    do_crab2();
    void                                    do_crab3();
    void                                    do_crab4();
    void                                    documentWasModified();
    void                                    newFile();
    void                                    print_prefix_signatures();
    void                                    print_suffix_signatures();
    void                                    read_file_do_crab();
    bool                                    save();
    bool                                    saveAs();

    void                                    read_stems_and_words();
    void                                    write_stems_and_words();

    // for find functionality
    void                                    launch_find_dock();

    // test for gold standard
    void                                    gs_read_and_parse_xml();
    void                                    gs_evaluate();
    void                                    read_morfessor_txt_file();
    void                                    gs_evaluate_morfessor();

#ifndef QT_NO_SESSIONMANAGER
    void                    commitData(QSessionManager &);
#endif

public:

    void createHorizontalGroupBox();
    void createSplitter();
    void ask_for_filename();
    void ask_for_project_file();
    void read_dx1_file();
    void read_text_file();

    void                    createFindDockWidget(UpperTableView* p_tableview);

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
    void                    create_or_update_TreeModel(CLexicon* lexicon);
    //                      update tree model after gs evaluation is completed
    void                    append_eval_results(EvaluationResults& results, QStandardItem* parent_item);
    void                    update_TreeModel_for_gs(CLexicon* lexicon);
    void                    update_TreeModel_for_eval(CLexicon* lexicon);

    void                    sort_upper_table();

    //                      windows

    void                    createActions();
    void                    createStatusBar();

    //                      settings

    void                    readSettings();
    void                    writeSettings();

    //                      working with files

    bool                    ask_to_save();
    bool                    saveFile(const QString &fileName);
    void                    setCurrentFile(const QString &fileName);
    QString                 strippedName(const QString &fullFileName);
    void                    cycle_through_graphic_displays();

    lxa_graphics_view*      get_graphics_view()                                 {return  m_graphics_view;}
    lxa_graphics_scene*     get_graphics_scene()                                {return m_graphics_scene;}
    UpperTableView *        get_upper_left_tableview()                          { return m_tableView_upper_left;}
    UpperTableView *        get_upper_right_tableview()                         { return m_tableView_upper_right;}
    void                    set_graphics_scene(lxa_graphics_scene* this_scene)  { m_graphics_scene = this_scene;}
    void                    set_lexicon(CLexicon* lexicon)                      { m_my_lexicon = lexicon;}
    void                    write_stems_and_affixes();

protected:
   void keyPressEvent(QKeyEvent *e) override;

};



#endif
