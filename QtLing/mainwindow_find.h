#ifndef MAINWINDOW_FINDDOCKWIDGET_H
#define MAINWINDOW_FINDDOCKWIDGET_H

#include <QObject>
#include <QWidget>
#include <QDockWidget>
#include <QLayout>
#include <QComboBox>
#include <QCheckBox>
#include "table_views.h"

class FindDialog;
class MainWindow;
class QCloseEvent;

/*!
 * \brief The FindDockWidget class. A wrapper object that contains a FindDialog.
 *
 * A QDockWidget is a small window-like interface that can be attached to the
 * margins of a QMainWindow object.
 */
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

/*!
 * \brief The FindDialog class. The main widget for implementing the search
 * functionality.
 *
 * The FindDialog object detects if its interactive components (buttons, etc.)
 * are activated through detecting signals from them to the FindDialog object's
 * slots. These slots then send signals to the UpperTableView objects so that
 * the TableView objects change their appearance in accordance with the search
 * request.
 */
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

    QLabel*             m_label_selection;
    QComboBox*          m_combobox_selection;
    QCheckBox*          m_checkbox_exact_match;
    QLineEdit*          m_line_edit;
    QLabel*             m_label_items_found;
    QPushButton*        m_button_find_next;
    QPushButton*        m_button_find_prev;
    QPushButton*        m_button_clear_search;
    QPushButton*        m_button_close;


    int                 m_left_items_found;
    int                 m_right_items_found;

private:
    void                connect_button_signals();
    void                connect_search_signals();

public:
    FindDialog(MainWindow* p_main_window);
    void                set_search_selection(QString s) {m_combobox_selection->setCurrentText(s);}
    bool                is_exact_match() { return m_checkbox_exact_match->isChecked(); }

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
