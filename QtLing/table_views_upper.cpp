#include <QDebug>
#include "table_views.h"
#include "lxamodels.h"



/**
* @brief UpperTableView::UpperTableView
* @param window
* This TableView responds to the User's click on the LeftSideTreeView.
*/
UpperTableView::UpperTableView (MainWindow* window, eSortStyle this_sort_style)
{
       m_parent_window = window;
       m_signature_sort_style = this_sort_style;
       m_gold_standard_display_order = 0;
       QFont sansFont("Ariel", 18);
       setFont(sansFont);
       setEditTriggers(QAbstractItemView::NoEditTriggers);

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
            set_data_type(e_data_gs_true_positive_parses);
        } else if (component == "Retrieved Parses") {
            set_data_type(e_data_gs_retrieved_parses);
        } else if (component == "Gold Standard Parses") {
            set_data_type(e_data_gs_gold_standard_parses);
        } else if (component == "Gold Standard Words") {
            component = "Gold Standard Parses";
            set_data_type(e_data_gs_gold_standard_parses);
        }

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
        // add component 10
    }



    resizeColumnsToContents();
}

bool UpperTableView::find_next_and_highlight(QString &s)
{
    qDebug() << "Signal to find next received; string to find: \"" + s + "\"";
    return false;
}

bool UpperTableView::find_prev_and_highlight(QString &s)
{
    qDebug() << "Signal to find next received; string to find: \"" + s + "\"";
    return false;
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

