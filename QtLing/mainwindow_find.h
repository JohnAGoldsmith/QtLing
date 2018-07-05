#ifndef MAINWINDOW_FINDDOCKWIDGET_H
#define MAINWINDOW_FINDDOCKWIDGET_H

#include <QObject>
#include <QWidget>
#include <QDockWidget>
#include <QLayout>
#include "table_views.h"

class FindDialog;

class FindDockWidget : public QDockWidget
{
    Q_OBJECT
    FindDialog*         m_child_dialog;
public:
    FindDockWidget(MainWindow* p_main_window, UpperTableView* p_tableview_searched = NULL);
    FindDialog*         get_child_dialog()      { return m_child_dialog; }
};

#endif // MAINWINDOW_FINDDOCKWIDGET_H

class FindDialog : public QWidget
{
    Q_OBJECT
    friend class FindDockWidget;
    typedef enum SearchRange {
        e_tableview_upper_all,
        e_tableview_upper_left,
        e_tableview_upper_right
    } SearchRange;

    QHBoxLayout*        m_layout;

    UpperTableView*     m_tableview_searched;
    SearchRange         m_search_range;
    MainWindow*         m_mainwindow;

    QLabel*             m_label;
    QLineEdit*          m_line_edit;
    QPushButton*        m_button_find_next;
    QPushButton*        m_button_find_prev;
    QPushButton*        m_clear_search;
    QPushButton*        m_close_button;

    QLabel*             m_items_found_label;
    bool                m_items_found_label_displayed;
    int                 m_left_items_found;
    int                 m_right_items_found;

private:
    void                connect_button_signals();
    void                connect_left_signals();
    void                connect_right_signals();
    void                connect_search_signals();

public:
    FindDialog(MainWindow* p_main_window, UpperTableView* p_tableview_searched = NULL);

signals:
    void                search_for_next(QString& s);
    void                search_for_prev(QString& s);
    void                clear_search();
public slots:
    void                do_next_search();
    void                do_prev_search();
    void                item_found(int n);
    void                do_clear_search();

};
