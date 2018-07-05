#ifndef MAINWINDOW_FINDDOCKWIDGET_H
#define MAINWINDOW_FINDDOCKWIDGET_H

#include <QObject>
#include <QWidget>
#include <QDockWidget>
#include <QLayout>
#include <QComboBox>
#include "table_views.h"

class FindDialog;
class MainWindow;
class QCloseEvent;

class FindDockWidget : public QDockWidget
{
    Q_OBJECT
    MainWindow*         m_parent_window;
    FindDialog*         m_child_dialog;
public:
    FindDockWidget(MainWindow* p_main_window);
    FindDialog*         get_child_dialog()      { return m_child_dialog; }
public slots:
    void                closeEvent(QCloseEvent *event);

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

    SearchRange         m_search_range;
    MainWindow*         m_mainwindow;

    QLabel*             m_search_label;
    QComboBox*          m_search_selection;
    QLineEdit*          m_line_edit;
    QPushButton*        m_button_find_next;
    QPushButton*        m_button_find_prev;
    QPushButton*        m_clear_search;
    QPushButton*        m_close_button;

    QLabel*             m_items_found_label;
    int                 m_left_items_found;
    int                 m_right_items_found;

private:
    void                connect_button_signals();
    void                connect_search_signals();

public:
    FindDialog(MainWindow* p_main_window);
    void                set_search_selection(QString s) {m_search_selection->setCurrentText(s);}

signals:
    void                search_for_left_next(QString& s);
    void                search_for_right_next(QString& s);
    void                search_for_left_prev(QString& s);
    void                search_for_right_prev(QString& s);
    void                clear_left_search();
    void                clear_right_search();
public slots:
    void                change_search_range(QString);
    void                do_next_search();
    void                do_prev_search();
    void                item_found(int n);
    void                do_clear_search();

};
