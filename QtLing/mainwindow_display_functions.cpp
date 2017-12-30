#include "mainwindow.h"
#include "graphics.h"


/**
 * @brief MainWindow::display_suffix_signatures
 * This is called by a QAction.
 */
void MainWindow::display_suffix_signatures()
{
    m_tableView_upper_left->setModel(m_Models["Signatures"]);
    m_tableView_upper_left->set_document_type( SIGNATURES );
    m_tableView_upper_left->resizeColumnsToContents();

    CLexicon* lexicon = get_lexicon();
    m_tableView_upper_right->setModel(m_Models["Signatures 2"]);
    m_tableView_upper_right->set_document_type( SIGNATURES );
    m_tableView_upper_right->resizeColumnsToContents();

    m_graphics_scene->clear_all();
    m_graphics_scene->assign_scene_positions_to_signatures(lexicon->get_signatures(), DT_All_Suffix_Signatures);
    m_graphics_scene->place_signatures();
}
/**
 * @brief MainWindow::display_prefix_signatures
 * This is called by a QAction.
 */
void MainWindow::display_prefix_signatures()
{
    m_tableView_upper_left->setModel(m_Models["Prefix signatures"]);
    m_tableView_upper_left->set_document_type( PREFIX_SIGNATURES );
    m_tableView_upper_left->resizeColumnsToContents();

    m_tableView_upper_right->setModel(m_Models["Prefix signatures 2"]);
    m_tableView_upper_right->set_document_type( PREFIX_SIGNATURES );
    m_tableView_upper_right->resizeColumnsToContents();

    m_graphics_scene->clear_all();
    CLexicon* lexicon = get_lexicon();
    m_graphics_scene->assign_scene_positions_to_signatures(lexicon->get_prefix_signatures(), DT_All_Prefix_Signatures);
    m_graphics_scene->place_signatures();
}
/**
 * @brief MainWindow::display_hypotheses
 * This is called by a QAction.
 */
void MainWindow::display_hypotheses()
{
    m_tableView_upper_left->setModel(m_Models["Hypotheses"]);
    m_tableView_upper_left->set_document_type( HYPOTHESES );
    m_tableView_upper_left->resizeColumnsToContents();

    m_tableView_upper_right->setModel(m_Models["Hypotheses 2"]);
    m_tableView_upper_right->set_document_type( HYPOTHESES_2 );
    m_tableView_upper_right->resizeColumnsToContents();


}
