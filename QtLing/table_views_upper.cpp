#include <QDebug>
#include <QMessageBox>
#include "table_views.h"
#include "lxamodels.h"
#include "mainwindow_find.h"



/**
* @brief UpperTableView::UpperTableView
* @param window
* This TableView responds to the User's click on the LeftSideTreeView.
*/
UpperTableView::UpperTableView (MainWindow* window, eSortStyle this_sort_style)
{
       m_parent_window = window;
       m_signature_sort_style = this_sort_style;

       QFont sansFont("Ariel", 18);
       setFont(sansFont);
       setEditTriggers(QAbstractItemView::NoEditTriggers);
       setSortingEnabled(true);

       m_row_recently_selected = -2;
       m_gold_standard_display_order = 0;
}



/**
 * @brief UpperTableView::ShowModelsUpperTableView
 * @param index
 * What is displayed in the UpperTableView is the direct result
 * of what item the user clicks on the LeftTree; that is the
 * input to this function.
 * This function chooses one of the many Qt models that
 * Crab generated when it had finished its work.
 *
 * I have set up two knds of UpperTableViews, so that they can be seen
 * side by side. They differ by sorting style. Signatures can be
 * sorted by the number of stems or the number of affixes, for example.
 *
 * TODO: Make the different sortings be done by a Surrogate QView sorter.
 * Right now I have created two different models for different sorts
 * of the Signatures, which isn't the right way to do things in principle. Dec 2017.
 * And I have had to do this several times.
 */
void UpperTableView::ShowModelsUpperTableView(const QModelIndex& index)
{
    CLexicon* lexicon = m_parent_window->get_lexicon();
    QString component;
    eDataType curr_data_type;

    // -- used for displaying gold standard information
    UpperTableView* left_table = m_parent_window->get_upper_left_tableview();
    UpperTableView* right_table = m_parent_window->get_upper_right_tableview();
    int& left_order = left_table->m_gold_standard_display_order;
    int& right_order = right_table->m_gold_standard_display_order;
    // --

    bool is_child_of_gs = false;

    if (index.isValid()){
        component = index.data().toString();
        QString parent_component = index.parent().data().toString();
        if (parent_component == "Gold Standard")
            is_child_of_gs = true;
    } else return;

    // Show gold standard information in tables
    if (is_child_of_gs && (component == "True Positive Parses"
     || component == "Gold Standard Parses"
     || component == "Retrieved Parses"
     || component == "Gold Standard Words")) {
        if (component == "True Positive Parses") {
            curr_data_type = e_data_gs_true_positive_parses;
        } else if (component == "Retrieved Parses") {
            curr_data_type = e_data_gs_retrieved_parses;
        } else if (component == "Gold Standard Parses") {
            curr_data_type = e_data_gs_gold_standard_parses;
        } else if (component == "Gold Standard Words") {
            component = "Gold Standard Parses";
            curr_data_type = e_data_gs_gold_standard_parses;
        }
        set_data_type(curr_data_type);

        if (left_order == 0 && right_order == 0) {
            setModel(m_parent_window->m_Models[component]);
            if (this == right_table)
                left_order = 1;
        } else if (left_order == 1 && right_order == 0) {
            if (this == right_table) {
                //qDebug() << "Showing " << component << "on the right";
                setModel(m_parent_window->m_Models[component]);
                left_order = 0;
                right_order = 1;
            }
        } else if (left_order == 0 && right_order == 1) {
            if (this == left_table) {
                setModel(m_parent_window->m_Models[component]);
                //qDebug() << "Showing " << component << "on the left";
            }
            if (this == right_table) {
                left_order = 1;
                right_order = 0;
            }
        } else {
            qDebug() << "UpperTableView::ShowModelsUpperTableView:"
                        "wrong case of gs display order!";
            left_order = 0;
            right_order = 0;
        }
    }
    // showing information in other tables
    else {
        left_table->m_gold_standard_display_order = 0;
        right_table->m_gold_standard_display_order = 0;

        if (component == "Words"){
            setModel(m_parent_window->m_Models["Words"]);
            set_data_type( e_data_words );
        }
        else     if (component == "Prefixal stems"){
            setModel(m_parent_window->m_Models["Prefixal stems"]);
            set_data_type(e_prefixal_stems);
        }
        else     if (component == "Suffixal stems"){
            setModel(m_parent_window->m_Models["Suffixal stems"]);
            set_data_type(e_suffixal_stems);
        }
        else     if (component == "Prefixes"){
            m_parent_window->display_prefixes();
            set_data_type(e_data_prefixes);
        }
        else     if (component == "Suffixes"){
            m_parent_window->display_suffixes();
            set_data_type(e_data_suffixes);
        }
        else     if (component == "Signatures"){
            m_parent_window->display_suffix_signatures(lexicon);
        }
        else     if (component == "EPositive signatures"){
            m_parent_window->display_epositive_suffix_signatures(lexicon);
        }
        else     if (component == "Prefix signatures"){
            m_parent_window->display_prefix_signatures(lexicon);
        }
        else     if (component == "EPositive prefix signatures"){
            m_parent_window->display_epositive_prefix_signatures(lexicon);
        }
        else     if (component == "Signature graph edges"){
            m_parent_window->display_signature_graph_edges(lexicon);
        }
        else     if (component == "Residual parasignatures"){

        }
        else     if (component == "Parasuffixes"){
            setModel(m_parent_window->m_Models["Parasuffixes"]);
            set_data_type ( e_data_suffixes );
            sortByColumn(1);
        }
        else     if (component == "Singleton signatures"){

        }
        else     if (component == "Passive signatures"){
            setModel(m_parent_window->m_Models["Passive signatures"]);
            set_data_type ( e_data_hollow_suffixal_signatures );
            sortByColumn(1);    }
        else     if (component == "Hypotheses"){
            m_parent_window->display_hypotheses();
        }
        else     if (component == "Suffixal protostems"){
            setModel(m_parent_window->m_Models["Suffixal protostems"]);
            set_data_type(e_data_suffixal_protostems);
            sortByColumn(1);
        }
        else     if (component == "Prefixal protostems"){
            setModel(m_parent_window->m_Models["Prefixal protostems"]);
            set_data_type(e_data_prefixal_protostems);
            sortByColumn(1);
        }
        // add component 10
    }

    resizeColumnsToContents();

    // Steps for making a sorted proxy model:
    // Using the model that is already set as the base model for a QSortFilterProxyModel
    // Reset proxy model as model of TableView

}

bool rows_less_than (const QStandardItem* item1, const QStandardItem* item2)
{
    return item1->row() < item2->row();
}

int UpperTableView::find_all_strings(const QString& str, bool exact_match)
{
    // qDebug() << "Finding strings";
    QStandardItemModel* p_model = (QStandardItemModel*) model();
    if (p_model == NULL) {
        // qDebug() << "Model not loaded";
        return 0;
    }
    m_items_found = QList<QStandardItem*>();
    m_items_found = p_model->findItems(str, exact_match? Qt::MatchExactly : Qt::MatchContains, 0);
    int num_items_found = m_items_found.length();
    if (num_items_found == 0) {
        // qDebug() << str << "was not found";
        return 0;
    } else {
        qSort(m_items_found.begin(), m_items_found.end(), rows_less_than);
        //Debug() << "Found" << num_items_found << "occurrences of" << str;
        QBrush brush_item_found(QColor(57, 197, 187));
        QStandardItem* p_item_found;
        foreach (p_item_found, m_items_found) {
            p_item_found->setBackground(brush_item_found);
        }
        return num_items_found;
    }
}

void UpperTableView::clear_search()
{
    QBrush brush(QColor(255, 255, 255));
    QStandardItemModel* p_model = (QStandardItemModel*) model();
    if (p_model == NULL) {
        qDebug() << "UpperTableView::clear_search(): model not loaded!";
        return;
    }
    for (int row_i = 0; row_i < p_model->rowCount(); row_i++) {
        p_model->item(row_i)->setBackground(brush);
    }
    m_items_found = QList<QStandardItem*>();
    clearSelection();
    m_row_recently_selected = -2;
}

bool UpperTableView::find_next_and_highlight(QString &s)
{
    // qDebug() << "Signal to find next received; string to find:" + s;
    QStandardItemModel* p_model = (QStandardItemModel*) model();
    FindDialog* p_find_dialog = (FindDialog*) sender();

    if (p_model == NULL) {
        qDebug() << "Model not loaded";
        m_row_recently_selected = -2;
        return false;
    }

    int num_found = find_all_strings(s, p_find_dialog->is_exact_match());
    if (num_found == 0) {
        emit num_items_found(0);
        m_row_recently_selected = -2;
        return false;
    }

    emit num_items_found(num_found);
    QModelIndexList sel_list = selectedIndexes();
    int sel_list_len = sel_list.length();
    int lowest_row;
    // see if the user has made a selection in the tableview
    if (sel_list_len != 0) {
        // get lowest row number among the items selected
        lowest_row = p_model->rowCount();
        QModelIndex index;
        foreach (index, sel_list) {
            int curr_row = index.row();
            if (curr_row < lowest_row)
                lowest_row = curr_row;
        }
        m_row_recently_selected = lowest_row;
    } else {
        // The user did not select anything; start from the beginning
        if (m_row_recently_selected == -2)
            m_row_recently_selected = -1;
    }

    // Highlight an item among the list of items found, this item may follow
    // after the user's selection or may be the first item in that list
    bool next_item_found = false;
    QList<QStandardItem*>::ConstIterator iter_item_found;
    int curr_row;
    for (iter_item_found = m_items_found.constBegin();
         iter_item_found != m_items_found.constEnd(); ) {
        curr_row = (*iter_item_found)->row();
        if (curr_row > m_row_recently_selected) {
            m_row_recently_selected = curr_row;
            next_item_found = true;
            break;
        }
        else
            iter_item_found++;
    }

    if (next_item_found) {
        selectRow(m_row_recently_selected);
        scrollTo(p_model->index(m_row_recently_selected,0));
        return true;
    } else {
        QMessageBox restart;
        if (this == m_parent_window->get_upper_left_tableview())
            restart.setText("Upper-left table: Search has reached the end.");
        else if (this == m_parent_window->get_upper_right_tableview())
            restart.setText("Upper-right table: Search has reached the end.");
        restart.setInformativeText("Start search again from the beginning?");
        restart.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        restart.setDefaultButton(QMessageBox::Yes);
        int rec = restart.exec();
        switch (rec) {
            case QMessageBox::Yes:
                m_row_recently_selected = -2;
                clearSelection();
                find_next_and_highlight(s);
                break;
            case QMessageBox::No:
                break;
            default:
                qDebug() << "table_views_upper.cpp: find_next_and_highlight()"
                            "- wrong switch case!";
        }
        return true;
    }
}

bool UpperTableView::find_prev_and_highlight(QString &s)
{
    QStandardItemModel* p_model = (QStandardItemModel*) model();
    FindDialog* p_find_dialog = (FindDialog*) sender();

    if (p_model == NULL) {
        qDebug() << "Model not loaded";
        m_row_recently_selected = -2;
        return false;
    }

    int num_found = find_all_strings(s, p_find_dialog->is_exact_match());
    if (num_found == 0) {
        emit num_items_found(0);
        m_row_recently_selected = -2;
        return false;
    }

    emit num_items_found(num_found);
    QModelIndexList sel_list = selectedIndexes();
    int sel_list_len = sel_list.length();
    int highest_row;
    // see if the user has made a selection in the tableview
    if (sel_list_len != 0) {
        // get lowest row number among the items selected
        highest_row = -1;
        QModelIndex index;
        foreach (index, sel_list) {
            int curr_row = index.row();
            if (curr_row > highest_row)
                highest_row = curr_row;
        }
        m_row_recently_selected = highest_row;
    } else {
        // The user did not select anything; start from the beginning
        if (m_row_recently_selected == -2)
            m_row_recently_selected = p_model->rowCount();
    }

    // Highlight an item among the list of items found, this item may follow
    // after the user's selection or may be the first item in that list
    bool next_item_found = false;
    QList<QStandardItem*>::const_reverse_iterator iter_item_found;
    int curr_row;
    for (iter_item_found = m_items_found.crbegin();
         iter_item_found != m_items_found.crend(); ) {
        curr_row = (*iter_item_found)->row();
        if (curr_row < m_row_recently_selected) {
            m_row_recently_selected = curr_row;
            next_item_found = true;
            break;
        }
        else
            iter_item_found++;
    }

    if (next_item_found) {
        selectRow(m_row_recently_selected);
        scrollTo(p_model->index(m_row_recently_selected,0));
        return true;
    } else {
        QMessageBox restart;
        restart.setText("Search has reached the top of the file.");
        restart.setInformativeText("Start search again from the bottom?");
        restart.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        restart.setDefaultButton(QMessageBox::Yes);
        int rec = restart.exec();
        switch (rec) {
            case QMessageBox::Yes:
                m_row_recently_selected = -2;
                clearSelection();
                find_prev_and_highlight(s);
                break;
            case QMessageBox::No:
                break;
            default:
                qDebug() << "table_views_upper.cpp: find_prev_and_highlight()"
                            "- wrong switch case!";
        }
        return true;
    }
}



/**
 * @brief UpperTableView::display_this_affixes_signatures
 * This is used when we click on the left upper view to identify an affix;
 * then we display all signatures containing that affix on the right.
 */
void  UpperTableView::display_this_affixes_signatures(const QModelIndex & index)
{
    if (index.isValid()){

    }

}

/**
* @brief UpperTableView::display_signatures
* When we click on an affix in the upper-table on the left,
* we display signatures on the right upper-table that contain that affix.
* We should just put that in a model, and link the model to that view.
*/
/*void UpperTableView::display_signatures()
{
   if (index.isValid()){
       row = index.row();
       column = index.column();
   }
   qDebug() << 145 <<  "table views suffixes";
   m_my_current_model->clear();
   CSignature* pSig;
   QStandardItem*        p_item;
   suffix_t suffix = index.sibling(row,0).data().toString();
   auto sig_iter (get_lexicon()->get_signatures()->get_map_iterator());
   while (sig_iter->hasNext()){
       pSig = sig_iter->next().value();
       if (pSig->contains_affix_string(suffix)){
           p_item = new QStandardItem(pSig->get_key());
           m_my_current_model->appendRow(p_item);
       }
   }
   get_parent_window()->get
}
*/
