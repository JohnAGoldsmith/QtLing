#ifndef TABLE_VIEWS_H
#define TABLE_VIEWS_H

#include "mainwindow.h"

class MainWindow;

class UpperTableView : public QTableView
{
    Q_OBJECT
    MainWindow*             m_parent_window;
    eDataType               m_data_type;     // this says the kind of thing in the table, so another view knows how to handle the information

    eSortStyle              m_signature_sort_style;

public:
    UpperTableView ();
    UpperTableView (MainWindow*, eSortStyle = DEFAULT);

    MainWindow*             get_parent_window()                         {return m_parent_window;}
    eDataType               get_data_type()                             {return m_data_type;}
    void                    set_data_type(eDataType type)               {m_data_type = type;}


public slots:
    eSortStyle              get_signature_sort_style ()                 { return m_signature_sort_style;}
    void                    set_signature_sort_style (eSortStyle style) {m_signature_sort_style = style;}
    void                    ShowModelsUpperTableView(const QModelIndex& );
    void                    display_this_affixes_signatures(const QModelIndex & index);

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

public slots:
        void                display_this_item(const QModelIndex&);
        CLexicon*           get_lexicon() {return m_lexicon;}
        int                 get_number_of_columns () {return m_number_of_columns;}
        void                graphics_sig_graph_edges(CSignature* , CLexicon*);
        void                table_signature(CSignature*) ;
        void                table_word (CWord*);
        //void                table_word(QString word, CLexicon* );
        void                table_one_signature(CSignature*, QStringList stem_list);
        void                table_passive_signature(CSignature*);
        void                table_stem(stem_t, CLexicon*);

signals:
};


class LeftSideTreeView : public QTreeView
{
    Q_OBJECT

    MainWindow * m_parent_window;

public:
    LeftSideTreeView(MainWindow*  window);
    void rowClicked(const QModelIndex& index);
};


#endif // TABLE_VIEWS_H
