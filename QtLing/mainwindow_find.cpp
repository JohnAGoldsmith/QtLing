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
    m_mainwindow(p_main_window)
{
    if (p_tableview_searched == p_main_window->get_upper_left_tableview()) {
        m_search_range = FindDialog::e_tableview_upper_left;
    } else if (p_tableview_searched == p_main_window->get_upper_right_tableview()) {
        m_search_range = FindDialog::e_tableview_upper_right;
    } else {
        m_search_range = FindDialog::e_tableview_upper_all;
    }

    m_layout = new QHBoxLayout;

    m_label = new QLabel;
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

    m_line_edit = new QLineEdit;
    m_line_edit->setMaximumWidth(400);

    m_button_find_next = new QPushButton(tr("Find Next"));

    m_button_find_prev = new QPushButton(tr("Find Previous"));

    //-- implementing close button
    // getting standard "x" icon fron style
    QStyle* style = qApp->style();
    QIcon close_icon = style->standardIcon(QStyle::SP_TitleBarCloseButton);
    m_close_button = new QPushButton;
    m_close_button->setIcon(close_icon);

    m_layout->addWidget(m_label);
    m_layout->addWidget(m_line_edit);
    m_layout->addWidget(m_button_find_next);
    m_layout->addWidget(m_button_find_prev);
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
}

void FindDialog::connect_search_signals()
{
    switch (m_search_range) {
        case e_tableview_upper_all:
            connect(this, SIGNAL(search_for_next(QString&)),
                    m_mainwindow->get_upper_right_tableview(),
                    SLOT(find_next_and_highlight(QString&)));
            connect(this, SIGNAL(search_for_prev(QString&)),
                    m_mainwindow->get_upper_right_tableview(),
                    SLOT(find_prev_and_highlight(QString&)));
        case e_tableview_upper_left:
            connect(this, SIGNAL(search_for_next(QString&)),
                    m_mainwindow->get_upper_left_tableview(),
                    SLOT(find_next_and_highlight(QString&)));
            connect(this, SIGNAL(search_for_prev(QString&)),
                    m_mainwindow->get_upper_left_tableview(),
                    SLOT(find_prev_and_highlight(QString&)));
            break;
        case e_tableview_upper_right:
            connect(this, SIGNAL(search_for_next(QString&)),
                    m_mainwindow->get_upper_right_tableview(),
                    SLOT(find_next_and_highlight(QString&)));
            connect(this, SIGNAL(search_for_prev(QString&)),
                    m_mainwindow->get_upper_right_tableview(),
                    SLOT(find_prev_and_highlight(QString&)));
            break;
        default:
        qDebug() << "mainwindow_find.cpp: connect_signals() - invalid search range!";
    }
}

void FindDialog::do_next_search()
{
    QString search_text = m_line_edit->text();
    emit search_for_next(search_text);
    qDebug() << "FindDialog: search_for_next(" + search_text + ") emitted!";
}

void FindDialog::do_prev_search()
{
    QString search_text = m_line_edit->text();
    emit search_for_prev(search_text);
    qDebug() << "FindDialog: search_for_prev(" + search_text + ") emitted!";
}
