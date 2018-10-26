#ifndef TABLE_VIEWS_H
#define TABLE_VIEWS_H

#include "mainwindow.h"
#include "lxamodels.h"

class MainWindow;
class CLexicon;
class CompoundWord;

class UpperTableView : public QTableView
{
    Q_OBJECT
    MainWindow*             m_parent_window;
    eDataType               m_data_type;     // this says the kind of thing in the table, so another view knows how to handle the information

    eSortStyle              m_signature_sort_style;

    int                     m_gold_standard_display_order;

    // for search functionality //
    int                     find_all_strings(const QString& str, bool exact_match);
    void                    clear_items_found();
    QList<QStandardItem*>   m_items_found;
    QModelIndexList         m_indeces_found;
    int                     m_row_recently_selected;
    void                    remap_indeces_and_highlight();
    static bool             qsi_row_less_than(const QStandardItem* item1, const QStandardItem* item2);
    static bool             index_row_less_than(const QModelIndex& i1, const QModelIndex& i2);

    LxaSortFilterProxyModel* m_proxy_model;

    // John Oct 2018
    void                    keyPressEvent(QKeyEvent *e);
    void                    createActions();

public:
    void                    showWords();
    void                    showSuffixSignatures();
 signals:
    void                    num_items_found(int);
    // for search functionality //


public:
    UpperTableView ();
    UpperTableView (MainWindow*, eSortStyle = DEFAULT);

    MainWindow*             get_parent_window()                         {return m_parent_window;}
    eDataType               get_data_type()                             {return m_data_type;}
    void                    set_data_type(eDataType type)               {m_data_type = type;}
    void                    set_proxy_source_model(QStandardItemModel* m) {m_proxy_model->setSourceModel(m);}

public slots:
    eSortStyle              get_signature_sort_style ()                 { return m_signature_sort_style;}
    void                    set_signature_sort_style (eSortStyle style) {m_signature_sort_style = style;}
    void                    ShowModelsUpperTableView(const QModelIndex& );
    void                    display_this_affixes_signatures(const QModelIndex & index);

    // for search functionality //
    bool                    find_prev_and_highlight(QString& s);
    bool                    find_next_and_highlight(QString& s);
    void                    clear_search();
    // for search functionality //

    void                    focusInEvent();
    //void                    keyPressEvent(QKeyEvent* e);

    signals:
    void                    please_display_this_signature(QString sig);
};


class LowerTableView : public QTableView
{
    Q_OBJECT
    MainWindow*             m_parent_window;
    int                     m_number_of_columns;
    QStandardItemModel*     m_my_current_model;
    CLexicon *              m_lexicon;
    int                     m_current_sortstyle;
    int                     m_number_of_sortstyles;

public:
    LowerTableView();
    LowerTableView(MainWindow*);
    MainWindow*             get_parent_window() {return m_parent_window;}
    void                    change_current_model(QStandardItemModel*);

    void                    keyPressEvent(QKeyEvent *e);

public slots:
        void                display_this_item(const QModelIndex&);
        CLexicon*           get_lexicon() {return m_lexicon;}
        int                 get_number_of_columns () {return m_number_of_columns;}
        void                graphics_sig_graph_edges(CSignature* , CLexicon*);
        void                table_signature(CSignature*) ;
        void                table_word (CWord*);
        //void                table_word(QString word, CLexicon* );
        void                table_one_signature(CSignature* pSig, QStringList stems, const QString& str_sig);
        void                table_passive_signature(CSignature*);
        void                table_stem(stem_t, CLexicon*);
        void                table_protostem(protostem* p_protostem);
        void                table_compound_composition(CompoundWord* p_compound, int composition_i);

signals:
};


class LeftSideTreeView : public QTreeView
{
    Q_OBJECT

    MainWindow * m_parent_window;

public:
    void    keyPressEvent(QKeyEvent * e);
    LeftSideTreeView(MainWindow*  window);
    void rowClicked(const QModelIndex& index);
};


#endif // TABLE_VIEWS_H
