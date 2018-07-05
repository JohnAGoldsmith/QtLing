#include "mainwindow_find.h"
#include <QApplication>

#include <QDockWidget>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIcon>
#include <QDebug>

FindDockWidget::FindDockWidget(MainWindow* p_main_window, UpperTableView* p_tableview_searched)
{
    setAllowedAreas(Qt::BottomDockWidgetArea);
    setFeatures(QDockWidget::DockWidgetMovable |
                QDockWidget::DockWidgetFloatable |
                QDockWidget::DockWidgetVerticalTitleBar);
    m_child_dialog = new FindDialog(p_main_window, p_tableview_searched);

    connect(m_child_dialog->m_close_button, SIGNAL(clicked(bool)),
            this, SLOT(close()));
    setWidget(m_child_dialog);
}

FindDialog::FindDialog(MainWindow *p_main_window, UpperTableView *p_tableview_searched):
    m_tableview_searched(p_tableview_searched),
    m_search_range(FindDialog::e_tableview_upper_all),
    m_mainwindow(p_main_window),
    m_items_found_label(NULL),
    m_items_found_label_displayed(false)
{
    if (p_tableview_searched == p_main_window->get_upper_left_tableview()) {
        m_search_range = FindDialog::e_tableview_upper_left;
    } else if (p_tableview_searched == p_main_window->get_upper_right_tableview()) {
        m_search_range = FindDialog::e_tableview_upper_right;
    } else {
        m_search_range = FindDialog::e_tableview_upper_all;
    }

    m_layout = new QHBoxLayout(this);

    m_label = new QLabel(this);
    switch (m_search_range) {
        case FindDialog::e_tableview_upper_all:
            m_label->setText(tr("Find in both upper tables:")); break;
        case FindDialog::e_tableview_upper_left:
            m_label->setText(tr("Find in upper left table:")); break;
        case FindDialog::e_tableview_upper_right:
            m_label->setText(tr("Find in upper right table:")); break;
        default:
            qDebug() << "mainwindow_find.cpp: FindDialog() - invalid search range!";
    }

    m_line_edit = new QLineEdit(this);
    m_line_edit->setMaximumWidth(300);

    m_button_find_next = new QPushButton(tr("Find Next"));

    m_button_find_prev = new QPushButton(tr("Find Previous"));

    m_clear_search = new QPushButton(tr("Clear Search"));

    //-- implementing close button
    // getting standard "x" icon fron style
    QStyle* style = qApp->style();
    QIcon close_icon = style->standardIcon(QStyle::SP_TitleBarCloseButton);
    m_close_button = new QPushButton(this);
    m_close_button->setIcon(close_icon);

    m_layout->addWidget(m_label);
    m_layout->addWidget(m_line_edit);
    m_layout->addWidget(m_button_find_next);
    m_layout->addWidget(m_button_find_prev);
    m_layout->addWidget(m_clear_search);
    m_layout->addWidget(m_close_button);
    m_layout->addStretch();

    setLayout(m_layout);
    connect_button_signals();
    connect_search_signals();
}

void FindDialog::connect_button_signals()
{
    connect(m_line_edit, SIGNAL(returnPressed()), this, SLOT(do_next_search()));
    connect(m_button_find_next, SIGNAL(clicked(bool)), this, SLOT(do_next_search()));
    connect(m_button_find_prev, SIGNAL(clicked(bool)), this, SLOT(do_prev_search()));
    connect(m_clear_search, SIGNAL(clicked(bool)), this, SLOT(do_clear_search()));
}

void FindDialog::connect_left_signals()
{
    connect(this, SIGNAL(search_for_next(QString&)),
            m_mainwindow->get_upper_left_tableview(),
            SLOT(find_next_and_highlight(QString&)));
    connect(this, SIGNAL(search_for_prev(QString&)),
            m_mainwindow->get_upper_left_tableview(),
            SLOT(find_prev_and_highlight(QString&)));
    connect(m_mainwindow->get_upper_left_tableview(),
            SIGNAL(num_items_found(int)),
            this, SLOT(item_found(int)));
    connect(this, SIGNAL(clear_search()),
            m_mainwindow->get_upper_left_tableview(),
            SLOT(clear_search()));
}

void FindDialog::connect_right_signals()
{
    connect(this, SIGNAL(search_for_next(QString&)),
            m_mainwindow->get_upper_right_tableview(),
            SLOT(find_next_and_highlight(QString&)));
    connect(this, SIGNAL(search_for_prev(QString&)),
            m_mainwindow->get_upper_right_tableview(),
            SLOT(find_prev_and_highlight(QString&)));
    connect(m_mainwindow->get_upper_right_tableview(),
            SIGNAL(num_items_found(int)),
            this, SLOT(item_found(int)));
    connect(this, SIGNAL(clear_search()),
            m_mainwindow->get_upper_right_tableview(),
            SLOT(clear_search()));
}

void FindDialog::connect_search_signals()
{
    switch (m_search_range) {
        case e_tableview_upper_all:
            connect_right_signals();
        case e_tableview_upper_left:
            connect_left_signals();
            break;
        case e_tableview_upper_right:
            connect_right_signals();
            break;
        default:
        qDebug() << "mainwindow_find.cpp: connect_signals() - invalid search range!";
    }
}

void FindDialog::do_next_search()
{
    QString search_text = m_line_edit->text();
    emit search_for_next(search_text);
    //qDebug() << "FindDialog: search_for_next(" + search_text + ") emitted!";
}

void FindDialog::do_prev_search()
{
    QString search_text = m_line_edit->text();
    emit search_for_prev(search_text);
    //qDebug() << "FindDialog: search_for_prev(" + search_text + ") emitted!";
}

void FindDialog::do_clear_search()
{
    if (m_items_found_label != NULL) {
        m_layout->removeWidget(m_items_found_label);
        m_items_found_label->hide();
        m_items_found_label_displayed = false;
    }
    m_line_edit->setText(QString());
    emit clear_search();
}

void FindDialog::item_found(int n)
{
    qDebug() << "item_found slot recieved signal with int n =" << n;
    if (sender() == m_mainwindow->get_upper_left_tableview())
        m_left_items_found = n;
    if (sender() == m_mainwindow->get_upper_right_tableview())
        m_right_items_found = n;

    if (m_items_found_label == NULL)
        m_items_found_label = new QLabel(this);

    QString label;
    switch (m_search_range) {
        case e_tableview_upper_all:
            label = QString("Occurrences found: Left - %1, Right - %2")
                    .arg(m_left_items_found).arg(m_right_items_found);
            break;
        case e_tableview_upper_left:
            label = QString("Occurrences found: Left - %1")
                    .arg(m_left_items_found);
            break;
        case e_tableview_upper_right:
            label = QString("Occurrences found: Right - %2")
                    .arg(m_right_items_found);
            break;
        default:
            qDebug() << "mainwindow_find.cpp: FindDialog::item_found - wrong switch case!";
    }

    m_items_found_label->setText(label);

    if (!m_items_found_label_displayed)
        m_layout->insertWidget(2, m_items_found_label);
    m_items_found_label_displayed = true;

}

/*
   if (!m_left_item_not_found && !m_right_item_not_found) {
        if (m_item_not_found_label != NULL) {
            m_layout->removeWidget(m_item_not_found_label);
            m_item_not_found_label->hide();
            m_item_not_found_label = NULL;
            m_item_not_found_label_displayed = false;
        }
    }
    */
