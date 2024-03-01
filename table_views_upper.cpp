#include <QDebug>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QTableView>
#include <algorithm>
#include "table_views.h"
#include "lxamodels.h"
#include "mainwindow_find.h"
#include "mainwindow.h"

//---------------------- New from 2023 and 2024 -------------------------------------------------------//









//---------------------------- Old from 2017 or so ---------------------------------------------------//

// This passes the user keyEvent up to the MainWindow, which responds to users
// general instructions.
// Careful: we need to pass on keyboard events that change the selected cell if we implement this.
//void UpperTableView::keyPressEvent(QKeyEvent *e)
//{
//   QTableView::keyPressEvent();
//    m_parent_window->MainWindow::keyPressEvent(e);
//}

void UpperTableView::createActions()
{
    /* --  Display Words -- */

}


void UpperTableView::showSuffixSignatures()
{
    setModel(m_parent_window->m_proxy_models["EPositive signatures"]);

    //m_proxy_model->setSourceModel(m_parent_window->m_Models["EPositive signatures"]);
    set_data_type( e_data_suffixal_signatures );
}

void UpperTableView::showPrefixSignatures()
{
    setModel(m_parent_window->m_proxy_models["EPositive prefix signatures"]);
    //m_proxy_model->setSourceModel(m_parent_window->m_Models["EPositive prefix signatures"]);
    set_data_type( e_data_prefixal_signatures );
}

void UpperTableView::showWords()
{
    setModel(m_parent_window->m_proxy_models["Words"]);
    //m_proxy_model->setSourceModel(m_parent_window->m_Models["Words"]);
    set_data_type( e_data_words );
}

/**
* @brief UpperTableView::UpperTableView
* @param window
* This TableView responds to the User's click on the LeftSideTreeView.
*/
UpperTableView::UpperTableView (MainWindow* window, eSortStyle this_sort_style)
{
       m_parent_window = window;
       m_signature_sort_style = this_sort_style;

       QFont sansFont("Ariel", 12); // was 18, july 2019 john goldsmith
       setFont(sansFont);
       setEditTriggers(QAbstractItemView::NoEditTriggers);
       setSortingEnabled(true);

       m_row_recently_selected = -2;
       m_gold_standard_display_order = 0;

       // -- use proxy model to allow sorting of data -- //
       /*
       m_proxy_model = new LxaSortFilterProxyModel(this);
       setModel(m_proxy_model);
       */
       // -- added by Hanson -- //

       setFocusPolicy(Qt::StrongFocus); // allows it to capture keystrokes
       createActions();
       setFocusPolicy(Qt::ClickFocus);
}


/* The following idea occurred to me:
 * This is a possible way to change the ShowModelsUpperTableView function:
 * Write a function that does a similar thing but take in a string and a pointer to a lexicon,
 * and dispays the corresponding table. In that way the display of tables can be modified
 * by other actions than the selecting a cell inside the left-side tree view.
 *
 * If we were to do this we need to change the current method of how evaluation info is displayed.
 *
 * Hanson 11.10
 *
 */


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
 *
 * Hanson's implementation of `LxaSortFilterProxyModel`:
 * [different models in main window] ---(source model of)---> [`m_proxy_model`]
 *                 [`m_proxy_model`]  ---(base model of)--->  [table view]
 * 1.   Call `setModel(m_proxy_model)` only once in the constructor function of
 *      the `UpperTableView` object. For each tableview there is only ONE
 *      `LxaSortFilterProxyModel` associated with it.
 * 2.   For showing a specific type of data (words, suffixes, signatures, etc),
 *      instead of using `setModel()` to change the table view, we use
 *      `setSourceModel()` to modify the `LxaSortFilterProxyModel` associated
 *      with it, so that different contents is rendered in the tableview.
 * comment added 8.1
 */
void UpperTableView::ShowModelsUpperTableView(const QModelIndex& index)
{
    //qDebug() << "137" << "Show models upperview old";
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
    /* The series of conditional jumps below have the following functionality.
     * Clicking on one of the items in goldstandard group in the left treeview
     * will have the following effect:
     * -    The first click will update both tableviews and display what is
     *      selected in both table views.
     * -    The second click will only update the right tableview and display
     *      the selected data in that tableview.
     * -    Subsequent clicks will only update and display content in the
     *      table view that is NOT most recently updated, e.g. will display
     *      "retrived parses" in the left tableview if the last update occured
     *      in the right tableview.
     */
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
        } else {
            // TODO: needs proper default type here.
            curr_data_type = e_data_gs_true_positive_parses; //not correct default case;
        }
        set_data_type(curr_data_type);

        if (left_order == 0 && right_order == 0) {
            setModel(m_parent_window->m_proxy_models[component]);
            //m_proxy_model->setSourceModel(m_parent_window->m_Models[component]);
            if (this == right_table)
                left_order = 1;
        } else if (left_order == 1 && right_order == 0) {
            if (this == right_table) {
                //qDebug() << "Showing " << component << "on the right";
                setModel(m_parent_window->m_proxy_models[component]);
                //m_proxy_model->setSourceModel(m_parent_window->m_Models[component]);
                left_order = 0;
                right_order = 1;
            }
        } else if (left_order == 0 && right_order == 1) {
            if (this == left_table) {
                setModel(m_parent_window->m_proxy_models[component]);
                //m_proxy_model->setSourceModel(m_parent_window->m_Models[component]);
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
    } /* --- end of displaying information related to Gold Standard ---
       * added by Hanson */
    // showing information in other tables
    else {
        left_table->m_gold_standard_display_order = 0;
        right_table->m_gold_standard_display_order = 0;

        if (component == "Words"){
            m_parent_window->display_words();            return;
        }
        if (component == "Suffixal sig pairs"){
            // ----------------- new -----------------------------------------------------
            m_parent_window->display_suffixal_sigpairs(m_parent_window->get_lexicon());
            return;
        }
        if (component == "Prefixal stems"){
            // ----------------- new -----------------------------------------------------
             m_parent_window->display_prefix_stems(m_parent_window->get_lexicon());
            // ----------------    old -----------------------------------------------------
            setModel(m_parent_window->m_proxy_models["Prefixal stems"]);
            //m_proxy_model->setSourceModel(m_parent_window->m_Models["Prefixal stems"]);
            set_data_type(e_prefixal_stems);
            return;
        }
        if (component == "Suffixal stems"){
            // ------------- new -----------------------------------------------------------
             m_parent_window->display_suffix_stems(m_parent_window->get_lexicon());

            // ---------------  old --------------------------------------------------------
            //setModel(m_parent_window->m_proxy_models["Suffixal stems"]);
            //set_data_type(e_suffixal_stems);
            //return;
            //--------------------------------------------------------------------------------
        }
        if (component == "Parasuffixes"){
            m_parent_window->display_parasuffixes(m_parent_window->get_lexicon());
        }
        if (component == "Prefixes"){
            m_parent_window->display_prefixes(m_parent_window->get_lexicon());
            set_data_type(e_data_prefixes);
            return;
        }
        if (component == "Suffixes"){
            m_parent_window->display_suffixes(m_parent_window->get_lexicon());
            set_data_type(e_data_suffixes);
            return;
        }
        if (component == "Suffixal signatures"){
            m_parent_window->display_suffix_signatures(lexicon);
            return;
        }
        if (component == "EPositive signatures"){
            m_parent_window->display_epositive_suffix_signatures(lexicon);
            return;
        }
        if (component == "Prefix signatures"){
            m_parent_window->display_prefix_signatures(lexicon);
            return;
        }
        if (component == "EPositive prefix signatures"){
            m_parent_window->display_epositive_prefix_signatures(lexicon);
            return;
        }
        if (component == "Signature graph edges"){
            m_parent_window->display_signature_graph_edges(lexicon);
            return;
        }
        if (component == "Parasuffixes"){
            setModel(m_parent_window->m_proxy_models["Parasuffixes"]);
            //m_proxy_model->setSourceModel(m_parent_window->m_Models["Parasuffixes"]);
            set_data_type ( e_data_suffixes );
            sortByColumn(1,Qt::AscendingOrder);
            return;
        }
        if (component == "Singleton signatures"){
            return;
        }
        if (component == "Passive signatures"){
            setModel(m_parent_window->m_proxy_models["Passive signatures"]);
            //m_proxy_model->setSourceModel(m_parent_window->m_Models["Passive signatures"]);
            set_data_type ( e_data_hollow_suffixal_signatures );
            sortByColumn(1,Qt::AscendingOrder);
            return;
        }
        if (component == "Hypotheses"){
            m_parent_window->display_hypotheses(lexicon);
            return;
        }
         if (component == "Suffixal protostems"){
            m_parent_window->display_suffixal_protostems(m_parent_window->get_lexicon());

            /*
            setModel(m_parent_window->m_proxy_models["Suffixal protostems"]);
            //m_proxy_model->setSourceModel(m_parent_window->m_Models["Suffixal protostems"]);
            set_data_type(e_data_suffixal_protostems);
            sortByColumn(1,Qt::AscendingOrder);
            */
            return;
        }
        if (component == "Prefixal protostems"){
            setModel(m_parent_window->m_proxy_models["Prefixal protostems"]);
            //m_proxy_model->setSourceModel(m_parent_window->m_Models["Prefixal protostems"]);
            set_data_type(e_data_prefixal_protostems);
            sortByColumn(1,Qt::AscendingOrder);
            return;
        }
        if (component == "Compound words"){
            setModel(m_parent_window->m_proxy_models["Compound words"]);
            //m_proxy_model->setSourceModel(m_parent_window->m_Models["Compound words"]);
            set_data_type(e_data_compound_words);
            sortByColumn(1,Qt::AscendingOrder);
            return;
        }
        if(component == "Parses"){
            // new:
             m_parent_window->display_parses(m_parent_window->get_lexicon());

            // old:
            setModel(m_parent_window->m_proxy_models["Parses"]);
            set_data_type(e_data_parses);
            sortByColumn(1,Qt::AscendingOrder);
            return;
        }  
    }
}

/*!
 * \brief Static compare function to compare two QStandardItems by their row numbers
 * \param item1
 * \param item2
 * \return
 *
 * Added by Hanson
 */
bool UpperTableView::qsi_row_less_than (const QStandardItem* item1, const QStandardItem* item2)
{
    return item1->row() < item2->row();
}

/*!
 * \brief Static compare function to compare two QModelIndex objects by their row numbers
 * \param item1
 * \param item2
 * \return
 *
 * Added by Hanson
 */
bool UpperTableView::index_row_less_than(const QModelIndex& i1, const QModelIndex& i2)
{
    return i1.row() < i2.row();
}

/*!
 * \brief Highlight items that are in the list of found items (m_items_found)
 * with a green color, and generate a list of QModelIndexes that contain all
 * the indeces of selected items, sorted in increasing row order.
 *
 * Called by `find_all_strings()`. Added by Hanson.
 */
void UpperTableView::remap_indeces_and_highlight()
{
    LxaSortFilterProxyModel* proxy_model = (LxaSortFilterProxyModel*) model();
    m_indices_found.clear();
    QStandardItem* item_found;
    QBrush brush_item_found(QColor(57, 197, 187));
    foreach (item_found, m_items_found) {
        item_found->setBackground(brush_item_found);
        m_indices_found.append(proxy_model->mapFromSource(item_found->index()));
    }

//  qSort(m_indeces_found.begin(), m_indeces_found.end(), index_row_less_than);
    std::sort(m_indices_found.begin(), m_indices_found.end(), index_row_less_than);
    //Debug() << "Found" << num_items_found << "occurrences of" << str;
}


/*!
 * \brief Given a string and match method, modifies the m_items_found list (a
 * member variable of UpperTableView) to store pointers to QStandardItem objects.
 * \param String to find
 * \param Whether search is exact match
 * \return Number of items found in the table.
 *
 * Currently only finds strings in the leftmost column. This function is called
 * by `find_next_and_highlight()` and `find_prev_and_highlight()`.
 * Added by Hanson.
 */
int UpperTableView::find_all_strings(const QString& str, bool exact_match)
{
    // qDebug() << "Finding strings";
    QStandardItemModel* p_model = (QStandardItemModel*) ((LxaSortFilterProxyModel*) model())->sourceModel();
    if (p_model == nullptr) {
        // qDebug() << "Model not loaded";
        return 0;
    }
    m_items_found = QList<QStandardItem*>();
    // the "0" indicates that the search is only done in the leftmost column.
    m_items_found = p_model->findItems(str, exact_match? Qt::MatchExactly : Qt::MatchContains, 0);

    int num_items_found = m_items_found.length();
    if (num_items_found == 0) {
        // qDebug() << str << "was not found";
        return 0;
    } else {
//        qSort(m_items_found.begin(), m_items_found.end(), qsi_row_less_than);
        std::sort(m_items_found.begin(), m_items_found.end(), qsi_row_less_than);
        remap_indeces_and_highlight();
        return num_items_found;
    }
}


/*!
 * \brief Clears highlighted colors, and any member variables that store search
 * results, and restores the search system to a blank, initial state.
 * Added by Hanson.
 */
void UpperTableView::clear_search()
{
    QBrush brush(QColor(255, 255, 255));
    QStandardItemModel* p_model = (QStandardItemModel*) ((LxaSortFilterProxyModel*) model())->sourceModel();
    if (p_model == nullptr) {
        qDebug() << "UpperTableView::clear_search(): model not loaded!";
        return;
    }
    for (int row_i = 0; row_i < p_model->rowCount(); row_i++) {
        p_model->item(row_i)->setBackground(brush);
    }
    m_items_found.clear();
    m_indices_found.clear();
    clearSelection();
    m_row_recently_selected = -2;
}

/*!
 * \brief Every time this slot is activated by a signal, calls `find_all
 * _strings()` to generate a list of found items in the tableview; selects and
 * scrolls down to the next item in the list of found items, relative to the
 * currently selected item.
 * \param s
 * \return false only if an error occurs.
 *
 * Selects the first found item in the list, with the lowest row number, if no
 * item is currently selected.
 * Added by Hanson.
 */
bool UpperTableView::find_next_and_highlight(QString &s)
{
    // qDebug() << "Signal to find next received; string to find:" + s;
    QStandardItemModel* p_model = (QStandardItemModel*) ((LxaSortFilterProxyModel*) model())->sourceModel();
    //QStandardItemModel* p_model = (QStandardItemModel*) m_proxy_model->sourceModel();
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
    QModelIndexList::ConstIterator iter_index_found;
    int curr_row;
    for (iter_index_found = m_indices_found.constBegin();
         iter_index_found != m_indices_found.constEnd(); ) {
        const QModelIndex& curr_index = *iter_index_found;
        curr_row = curr_index.row();
        if (curr_row > m_row_recently_selected) {
            m_row_recently_selected = curr_row;
            next_item_found = true;
            break;
        }
        else
            iter_index_found++;
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

/*!
 * \brief Every time this slot is activated by a signal, calls `find_all
 * _strings()` to generate a list of found items in the tableview; selects and
 * scrolls up to the previous item in the list of found items, relative to the
 * currently selected item.
 * \param s
 * \return false only if an error occurs.
 *
 * Selects the last found item in the list, with the highest row number, if no
 * item is currently selected.
 * Added by Hanson.
 */
bool UpperTableView::find_prev_and_highlight(QString &s)
{
    QStandardItemModel* p_model = (QStandardItemModel*) ((LxaSortFilterProxyModel*) model())->sourceModel();
    //QStandardItemModel* p_model = (QStandardItemModel*) m_proxy_model->sourceModel();
    FindDialog* p_find_dialog = (FindDialog*) sender();

    if (p_model == nullptr) {
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
        // get highest row number among the items selected
        highest_row = -1;
        QModelIndex index;
        foreach (index, sel_list) {
            int curr_row = index.row();
            if (curr_row > highest_row)
                highest_row = curr_row;
        }
        m_row_recently_selected = highest_row;
    } else {
        // The user did not select anything; start from the end
        if (m_row_recently_selected == -2)
            m_row_recently_selected = p_model->rowCount();
    }

    // Highlight an item among the list of items found, this item may follow
    // after the user's selection or may be the first item in that list
    bool next_item_found = false;
    QModelIndexList::reverse_iterator iter_index_found;
    int curr_row;
    for (iter_index_found = m_indices_found.rbegin();
         iter_index_found != m_indices_found.rend(); ) {
        const QModelIndex& curr_index = *iter_index_found;
        curr_row = curr_index.row();
        if (curr_row < m_row_recently_selected) {
            m_row_recently_selected = curr_row;
            next_item_found = true;
            break;
        }
        else
            iter_index_found++;
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
        int result = restart.exec();
        switch (result) {
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

void UpperTableView::focusInEvent()
{
    qDebug() << "Upper table view focused";
}
