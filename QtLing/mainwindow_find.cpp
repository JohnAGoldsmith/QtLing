#include "mainwindow_find.h"
#include <QApplication>

#include <QDockWidget>
#include <QLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIcon>
#include <QDebug>

#include "mainwindow.h"

FindDockWidget::FindDockWidget(MainWindow* p_main_window)
{
    setAllowedAreas(Qt::BottomDockWidgetArea);
    setFeatures(QDockWidget::DockWidgetClosable |
                QDockWidget::DockWidgetVerticalTitleBar);
    m_parent_window = p_main_window;
    m_child_dialog = new FindDialog(p_main_window);

    connect(m_child_dialog->m_button_close, SIGNAL(clicked(bool)),
            this, SLOT(close()));
    setWidget(m_child_dialog);
}

void FindDockWidget::closeEvent(QCloseEvent *event)
{
    m_child_dialog->do_clear_search();
    event->accept();
}

FindDialog::FindDialog(MainWindow *p_main_window):
    m_search_range(FindDialog::e_tableview_upper_all),
    m_mainwindow(p_main_window),
    m_label_items_found(NULL)
{
    m_layout = new QHBoxLayout(this);

    m_label_selection = new QLabel(tr("Find in:"), this);

    m_combobox_selection = new QComboBox(this);
    m_combobox_selection->addItem(tr("Both upper tables"));
    m_combobox_selection->addItem(tr("Upper-left table"));
    m_combobox_selection->addItem(tr("Upper-right table"));

    int search_sel_init_index;
    switch (m_search_range) {
        case FindDialog::e_tableview_upper_all:
            search_sel_init_index = 0; break;
        case FindDialog::e_tableview_upper_left:
            search_sel_init_index = 1; break;
        case FindDialog::e_tableview_upper_right:
            search_sel_init_index = 2; break;
        default:
            qDebug() << "mainwindow_find.cpp: FindDialog() - invalid search range!";
    }
    m_checkbox_exact_match = new QCheckBox(tr("Exact Match?"),this);
    m_checkbox_exact_match->setChecked(false);

    m_line_edit = new QLineEdit(this);
    m_line_edit->setMaximumWidth(300);

    m_label_items_found = new QLabel(this);

    m_button_find_next = new QPushButton(tr("Find Next"));

    m_button_find_prev = new QPushButton(tr("Find Previous"));

    m_button_clear_search = new QPushButton(tr("Clear Search"));

    //-- implementing close button
    // getting standard "x" icon fron style
    QStyle* style = qApp->style();
    QIcon close_icon = style->standardIcon(QStyle::SP_TitleBarCloseButton);
    m_button_close = new QPushButton(this);
    m_button_close->setIcon(close_icon);

    m_layout->addWidget(m_label_selection);
    m_layout->addWidget(m_combobox_selection);
    m_layout->addWidget(m_checkbox_exact_match);
    m_layout->addWidget(m_line_edit);
    m_layout->addWidget(m_label_items_found);
    m_label_items_found->hide();
    m_layout->addWidget(m_button_find_next);
    m_layout->addWidget(m_button_find_prev);
    m_layout->addWidget(m_button_clear_search);
    m_layout->addWidget(m_button_close);
    m_layout->addStretch();

    setLayout(m_layout);
    connect_button_signals();
    connect_search_signals();
}

void FindDialog::connect_button_signals()
{
    connect(m_combobox_selection, SIGNAL(currentIndexChanged(QString)), this, SLOT(change_search_range(QString)));
    connect(m_line_edit, SIGNAL(returnPressed()), this, SLOT(do_next_search()));
    connect(m_button_find_next, SIGNAL(clicked(bool)), this, SLOT(do_next_search()));
    connect(m_button_find_prev, SIGNAL(clicked(bool)), this, SLOT(do_prev_search()));
    connect(m_button_clear_search, SIGNAL(clicked(bool)), this, SLOT(do_clear_search()));
    connect(m_line_edit, SIGNAL(textChanged(QString)), this, SLOT(do_clear_search()));
}

void FindDialog::connect_search_signals()
{
    connect(this, SIGNAL(search_for_left_next(QString&)),
            m_mainwindow->get_upper_left_tableview(),
            SLOT(find_next_and_highlight(QString&)));
    connect(this, SIGNAL(search_for_left_prev(QString&)),
            m_mainwindow->get_upper_left_tableview(),
            SLOT(find_prev_and_highlight(QString&)));
    connect(m_mainwindow->get_upper_left_tableview(),
            SIGNAL(num_items_found(int)),
            this, SLOT(item_found(int)));
    connect(this, SIGNAL(clear_left_search()),
            m_mainwindow->get_upper_left_tableview(),
            SLOT(clear_search()));

    connect(this, SIGNAL(search_for_right_next(QString&)),
            m_mainwindow->get_upper_right_tableview(),
            SLOT(find_next_and_highlight(QString&)));
    connect(this, SIGNAL(search_for_right_prev(QString&)),
            m_mainwindow->get_upper_right_tableview(),
            SLOT(find_prev_and_highlight(QString&)));
    connect(m_mainwindow->get_upper_right_tableview(),
            SIGNAL(num_items_found(int)),
            this, SLOT(item_found(int)));
    connect(this, SIGNAL(clear_right_search()),
            m_mainwindow->get_upper_right_tableview(),
            SLOT(clear_search()));
}

void FindDialog::change_search_range(QString s)
{
    // qDebug() << "FindDialog::change_search_range: received change search range signal - " << s;
    if (s == "Both upper tables")
        m_search_range = e_tableview_upper_all;
    else if (s == "Upper-left table")
        m_search_range = e_tableview_upper_left;
    else if (s == "Upper-right table")
        m_search_range = e_tableview_upper_right;
    else
        qDebug() << "FindDialog::change_search_range: wrong text in combobox!";
}

void FindDialog::do_next_search()
{
    QString search_text = m_line_edit->text();
    switch (m_search_range) {
        case e_tableview_upper_all:
            emit search_for_right_next(search_text);
        case e_tableview_upper_left:
            emit search_for_left_next(search_text);
            break;
        case e_tableview_upper_right:
            emit search_for_right_next(search_text);
            break;
        default:
        qDebug() << "mainwindow_find.cpp: connect_signals() - invalid search range!";
    }

    //qDebug() << "FindDialog: search_for_next(" + search_text + ") emitted!";
}

void FindDialog::do_prev_search()
{
    QString search_text = m_line_edit->text();
    switch (m_search_range) {
        case e_tableview_upper_all:
            emit search_for_right_prev(search_text);
        case e_tableview_upper_left:
            emit search_for_left_prev(search_text);
            break;
        case e_tableview_upper_right:
            emit search_for_right_prev(search_text);
            break;
        default:
        qDebug() << "mainwindow_find.cpp: connect_signals() - invalid search range!";
    }
    //qDebug() << "FindDialog: search_for_prev(" + search_text + ") emitted!";
}

void FindDialog::do_clear_search()
{
    m_label_items_found->hide();
    if (sender() == m_button_clear_search) {
        m_line_edit->setText(QString());
        emit clear_left_search();
        emit clear_right_search();
    } else if (sender() == m_line_edit) {
        switch (m_search_range) {
            case e_tableview_upper_all:
                emit clear_right_search();
            case e_tableview_upper_left:
                emit clear_left_search();
                return;
            case e_tableview_upper_right:
                emit clear_right_search();
                return;
            default:
            qDebug() << "mainwindow_find.cpp:do_clear_search() - invalid search range!";
        }
    } else {
        emit clear_left_search();
        emit clear_right_search();
    }
}

void FindDialog::item_found(int n)
{
    //qDebug() << "item_found slot recieved signal with int n =" << n;
    if (sender() == m_mainwindow->get_upper_left_tableview())
        m_left_items_found = n;
    if (sender() == m_mainwindow->get_upper_right_tableview())
        m_right_items_found = n;

    QString label;
    switch (m_search_range) {
        case e_tableview_upper_all:
            label = QString("Occurrences found: [Left] %1, [Right] %2")
                    .arg(m_left_items_found).arg(m_right_items_found);
            break;
        case e_tableview_upper_left:
            label = QString("Occurrences found: [Left] %1")
                    .arg(m_left_items_found);
            break;
        case e_tableview_upper_right:
            label = QString("Occurrences found: [Right] %2")
                    .arg(m_right_items_found);
            break;
        default:
            qDebug() << "mainwindow_find.cpp: FindDialog::item_found - wrong switch case!";
    }

    m_label_items_found->setText(label);
    m_label_items_found->show();

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
