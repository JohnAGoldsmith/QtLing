#ifndef TABLE_VIEWS_H
#define TABLE_VIEWS_H

#include "mainwindow.h"

class MainWindow;

class UpperTableView : public QTableView
{
    Q_OBJECT
    QString         m_content;
    MainWindow*     m_parent_window;
    eDocumentType   m_document_type;
    eSortStyle      m_signature_sort_style;
public:
    UpperTableView ();
    UpperTableView (MainWindow*, eSortStyle = DEFAULT);
    QString         get_content()                   {return m_content;}
    void            set_content_type(QString text)  {m_content = text;}
    MainWindow*     get_parent_window()             {return m_parent_window;}
    eDocumentType   get_document_type()             {return m_document_type;}
    void            set_document_type(eDocumentType type) {m_document_type = type;}
public slots:
    void ShowModelsUpperTableView(const QModelIndex& );
    eSortStyle      get_signature_sort_style () { return m_signature_sort_style;}
    void            set_signature_sort_style (eSortStyle style) {m_signature_sort_style = style;}
    void            display_this_affixes_signatures(const QModelIndex& );

    signals:
    void please_display_this_signature(QString sig);
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
    //CLexicon * p_lexicon;
    MainWindow* get_parent_window() {return m_parent_window;}
    void change_current_model(QStandardItemModel*);
public slots:
        void display_this_item(const QModelIndex&);
        void table_signature(CSignature*) ;
        void table_word (CWord*);
        void table_one_signature(CSignature*, QStringList stem_list);
        void table_passive_signature(CSignature*);
        void graphics_sig_graph_edges(CSignature* , CLexicon*);
        int  get_number_of_columns () {return m_number_of_columns;}
        CLexicon* get_lexicon() {return m_lexicon;}
        //void    advance_sortstyle() {m_sort}

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


#endif // TABLE_VIEWS_H
