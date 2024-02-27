#include <QStandardItemModel>

#include "mainwindow.h"
#include "graphics.h"
#include "lxamodels.h"
#include "Lexicon.h"
#include "parsemodel.h"

void MainWindow::display_words()
{
    //New December 29 2023
    if (m_word_model) {delete m_word_model;}
    m_word_model = new wordmodel(m_my_lexicon->get_word_collection());


    //m_word_model_proxy_1->setSourceModel(m_word_model);
    QSortFilterProxyModel * proxy_model = new QSortFilterProxyModel();
    proxy_model->setSourceModel(m_word_model);

    //m_tableView_upper_temp->setModel(m_word_model_proxy_1);
    m_tableView_upper_temp->setModel(proxy_model);
    // ------------ old --------------------------------------------------
    /*
    m_tableView_upper_left->setModel(m_proxy_models["Words"]);
    m_tableView_upper_left->set_data_type ( e_data_words );
    m_tableView_upper_left->resizeColumnsToContents();

    m_tableView_upper_right->setModel(m_proxy_models["Words 2"]);
    m_tableView_upper_right->set_data_type ( e_data_words );
    m_tableView_upper_right->resizeColumnsToContents();
    */
}
void MainWindow::display_parses(CLexicon* lexicon){
    m_parse_model = new parsemodel(m_my_lexicon->get_parses(), this);
    m_tableView_upper_temp->setModel(m_parse_model);
}
void MainWindow::display_suffix_stems(CLexicon* lexicon){
    if (m_suffixal_stem_model) {delete m_suffixal_stem_model;}
    m_suffixal_stem_model = new stemmodel(m_my_lexicon->get_suffixal_stems());
    m_stem_model_proxy_1->setSourceModel(m_suffixal_stem_model);
    m_tableView_upper_temp->setModel(m_stem_model_proxy_1);
}


void MainWindow::display_prefix_stems(CLexicon* lexicon){
    if (m_prefixal_stem_model) {delete m_prefixal_stem_model;}
    m_prefixal_stem_model = new stemmodel(m_my_lexicon->get_prefixal_stems());
    m_tableView_upper_temp->setModel(m_prefixal_stem_model);
}
void MainWindow::display_suffixal_sigpairs(CLexicon* lexicon){
   // if (m_suffixal_sigpair_model) {delete m_suffixal_sigpair_model;} // when I leave this in, it createsa a problem. TO DO! TODO
    m_suffixal_sigpair_model = new SigPairModel(m_my_lexicon->get_suffixal_sig_pairs());
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(m_suffixal_sigpair_model);
    m_tableView_upper_temp->setModel(proxyModel);
    m_tableView_upper_temp->set_data_type(e_sig_pairs);
    //m_tableView_upper_temp->setModel(m_suffixal_sigpair_model);
}
void MainWindow::display_parasuffixes(CLexicon* lexicon)
{   // new
    Q_UNUSED (lexicon)

    if (m_suffix_model) {delete m_suffix_model;}
    m_suffix_model = new affixmodel(get_lexicon()->get_parasuffixes()->get_suffix_list());
    m_affix_model_proxy_1->setSourceModel(m_suffix_model);
    m_tableView_upper_temp->setModel(m_affix_model_proxy_1);


}
/**
 * @brief MainWindow::display_hypotheses
 * This is called by a QAction.
 */
void MainWindow::display_hypotheses(CLexicon* lexicon)
{
    // new



    // old
    m_tableView_upper_left->setModel(m_proxy_models["Hypotheses 2"]);
    //m_tableView_upper_left->set_proxy_source_model(m_Models["Hypotheses"]);
    m_tableView_upper_left->set_data_type ( e_data_hypotheses );
    m_tableView_upper_left->resizeColumnsToContents();
    m_tableView_upper_left->model()->sort(4, Qt::DescendingOrder); // word count column

    //m_tableView_upper_left->setModel(m_proxy_models["Hypotheses 2"]);
    //m_tableView_upper_right->set_proxy_source_model(m_Models["Hypotheses 2"]);
    m_tableView_upper_right->set_data_type ( e_data_hypotheses );
    m_tableView_upper_right->resizeColumnsToContents();


}


/**
 * @brief MainWindow::display_suffix_signatures
 * This is called by a QAction.
 */
void MainWindow::display_suffix_signatures(CLexicon* lexicon)
{   // new
    if (m_suffix_signature_model){delete m_suffix_signature_model;}
    m_suffix_signature_model = new signaturemodel(m_my_lexicon->get_signatures());
    m_signature_model_proxy_1->setSourceModel(m_suffix_signature_model);
    m_tableView_upper_temp->setModel(m_signature_model_proxy_1);


    // old
    m_tableView_upper_left->setModel(m_proxy_models["Suffixal signatures"]);
    m_tableView_upper_left->set_data_type( e_data_suffixal_signatures );
    m_tableView_upper_left->model()->sort(2, Qt::DescendingOrder);
    m_tableView_upper_left->resizeColumnsToContents();

    m_tableView_upper_right->setModel(m_proxy_models["Signatures 2"]);
    m_tableView_upper_right->set_data_type( e_data_suffixal_signatures );
    m_tableView_upper_right->resizeColumnsToContents();
    m_tableView_upper_right->model()->sort(0, Qt::DescendingOrder);

    m_graphics_scene->clear_all();
    m_graphics_scene->assign_lattice_positions_to_signatures(lexicon->get_suffix_signatures(), e_data_suffixal_signatures);
    m_graphics_scene->create_and_place_signatures();

}
/**
 * @brief MainWindow::display_prefix_signatures
 * This is called by a QAction.
 */
void MainWindow::display_prefix_signatures(CLexicon* lexicon)
{   //------------------ new ----------------------------------------------------
    m_signature_model_proxy_1->setSourceModel(m_prefix_signature_model);
    m_tableView_upper_temp->setModel(m_signature_model_proxy_1);


    // ---------------- old ----------------------------------------------------
    m_tableView_upper_left->setModel(m_proxy_models["Prefix signatures"]);
    //m_tableView_upper_left->set_proxy_source_model(m_Models["Prefix signatures"]);
    m_tableView_upper_left->set_data_type ( e_data_prefixal_signatures );
    m_tableView_upper_left->resizeColumnsToContents();
    m_tableView_upper_left->model()->sort(2, Qt::DescendingOrder);

    m_tableView_upper_right->setModel(m_proxy_models["Prefix signatures 2"]);
    //m_tableView_upper_right->set_proxy_source_model(m_Models["Prefix signatures 2"]);
    m_tableView_upper_right->set_data_type ( e_data_prefixal_signatures );
    m_tableView_upper_right->resizeColumnsToContents();
    m_tableView_upper_right->model()->sort(0, Qt::DescendingOrder);

    m_graphics_scene->clear_all();
    m_graphics_scene->assign_lattice_positions_to_signatures(lexicon->get_prefix_signatures(), e_data_prefixal_signatures);
    m_graphics_scene->create_and_place_signatures();

}









/**
 * @brief MainWindow::display_suffixes
 * This is called by a QAction.
 */
void MainWindow::display_suffixes(CLexicon* lexicon)
{   // new
    Q_UNUSED (lexicon)

    if (m_suffix_model) {delete m_suffix_model;}
    m_suffix_model = new affixmodel(get_lexicon()->get_suffixes()->get_suffix_list());
    m_affix_model_proxy_1->setSourceModel(m_suffix_model);
    m_tableView_upper_temp->setModel(m_affix_model_proxy_1);

    // old
    m_tableView_upper_left->setModel(m_proxy_models["Suffixes"]);
    m_tableView_upper_left->set_data_type ( e_data_suffixes );
    m_tableView_upper_left->resizeColumnsToContents();

    m_tableView_upper_right->setModel(m_proxy_models["Suffixes 2"]);
    m_tableView_upper_right->set_data_type ( e_data_suffixes );
    m_tableView_upper_right->resizeColumnsToContents();
 }

/**
 * @brief MainWindow::display_prefixes
 * This is called by a QAction.
 */
void MainWindow::display_prefixes(CLexicon * lexicon)
{
    Q_UNUSED (lexicon);
    // new
    Q_UNUSED (lexicon)
    m_affix_model_proxy_1->setSourceModel(m_prefix_model);
    m_tableView_upper_temp->setModel(m_affix_model_proxy_1);

    // old
    m_tableView_upper_left->setModel(m_proxy_models["Prefixes"]);
    //m_tableView_upper_left->set_proxy_source_model(m_Models["Prefixes"]);
    m_tableView_upper_left->set_data_type ( e_data_prefixes );
    m_tableView_upper_left->resizeColumnsToContents();

    m_tableView_upper_right->setModel(m_proxy_models["Prefixes 2"]);
    //m_tableView_upper_left->set_proxy_source_model(m_Models["Prefixes"]);
    m_tableView_upper_right->set_data_type ( e_data_prefixes );
    m_tableView_upper_right->resizeColumnsToContents();
 }
void MainWindow::display_suffixal_protostems(CLexicon* lexicon){
    if (m_suffixal_protostem_model){delete m_suffixal_protostem_model;}
    m_suffixal_protostem_model = new stemmodel(lexicon->get_suffixal_protostems());
    m_tableView_upper_temp->setModel(m_suffixal_protostem_model);

}
/**
 * @brief MainWindow::display_epositive_suffix_signatures
 */
void MainWindow::display_epositive_suffix_signatures(CLexicon*  )
{
    if (m_Models["EPositive signatures"]->rowCount() > 0){
        m_tableView_upper_left->setModel(m_proxy_models["EPositive signatures"]);
        //m_tableView_upper_left->set_proxy_source_model(m_Models["EPositive signatures"]);
        m_tableView_upper_left->set_data_type(e_data_epositive_suffixal_signatures );
        m_tableView_upper_left->model()->sort(2, Qt::DescendingOrder);

        m_tableView_upper_right->setModel(m_proxy_models["EPositive signatures 2"]);
        //m_tableView_upper_right->set_proxy_source_model(m_Models["EPositive signatures 2"]);
        m_tableView_upper_right->set_data_type( e_data_epositive_suffixal_signatures );
        m_tableView_upper_right->resizeColumnsToContents();
        m_tableView_upper_right->model()->sort(0, Qt::DescendingOrder);

        m_graphics_scene->assign_lattice_positions_to_signatures(get_lexicon()->get_signatures(), e_data_epositive_suffixal_signatures);
        m_graphics_scene->create_and_place_signatures();
    }
}
void MainWindow::display_epositive_prefix_signatures(CLexicon* lexicon)
{
    if (m_Models["EPositive prefix signatures"]->rowCount() > 0){
        m_tableView_upper_left->setModel(m_proxy_models["EPositive prefix signatures"]);
        //m_tableView_upper_left->set_proxy_source_model(m_Models["EPositive prefix signatures"]);
        m_tableView_upper_left->set_data_type(e_data_epositive_prefixal_signatures );
        m_tableView_upper_left->set_data_type(e_data_epositive_prefixal_signatures);
        m_tableView_upper_left->model()->sort(2, Qt::DescendingOrder);

        m_tableView_upper_right->setModel(m_proxy_models["EPositive prefix signatures 2"]);
        //m_tableView_upper_right->set_proxy_source_model(m_Models["EPositive prefix signatures 2"]);
        m_tableView_upper_right->set_data_type( e_data_epositive_prefixal_signatures );
        m_tableView_upper_right->resizeColumnsToContents();
        m_tableView_upper_right->model()->sort(0, Qt::DescendingOrder);

        m_graphics_scene->clear_all();
        m_graphics_scene->assign_lattice_positions_to_signatures(lexicon->get_prefix_signatures(), e_data_epositive_suffixal_signatures);
        m_graphics_scene->create_and_place_signatures();
    }
}

void MainWindow::display_signature_graph_edges(CLexicon* ){
    m_tableView_upper_left->setModel(m_proxy_models["SigGraphEdges_1"]);
    //m_tableView_upper_left->set_proxy_source_model(m_Models["SigGraphEdges_1"]);
    m_tableView_upper_left->set_data_type( e_data_signatures_graph_edges );
    m_tableView_upper_left->resizeColumnsToContents();

    m_tableView_upper_right->setModel(m_proxy_models["SigGraphEdges_2"]);
    //m_tableView_upper_right->set_proxy_source_model(m_Models["SigGraphEdges_2"]);
    m_tableView_upper_right->set_data_type( e_data_signatures_graph_edges );
    m_tableView_upper_right->resizeColumnsToContents();



}




