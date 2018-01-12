#include <QStandardItemModel>

#include "mainwindow.h"
#include "graphics.h"
#include "lxamodels.h"

/**
 * @brief MainWindow::display_suffix_signatures
 * This is called by a QAction.
 */
void MainWindow::display_suffixes()
{
    m_tableView_upper_left->setModel(m_Models["Suffixes"]);
    m_tableView_upper_left->set_display_type ( DT_affixes );
    m_tableView_upper_left->resizeColumnsToContents();
 }

/**
 * @brief MainWindow::display_epositive_suffix_signatures
 */
void MainWindow::display_epositive_suffix_signatures()
{

    m_tableView_upper_left->setModel(m_Models["EPositive signatures"]);
    m_tableView_upper_left->set_display_type(DT_Positive_Suffix_Signatures );
    m_tableView_upper_left->set_content_type( "full suffixal signatures");

    set_current_graphics_scene (get_suffix_graphics_scene());
    m_current_graphics_scene->clear_all();
    m_current_graphics_scene->assign_scene_positions_to_signatures(get_lexicon()->get_signatures(), DT_Positive_Suffix_Signatures);
    m_current_graphics_scene->place_signatures();
}

/**
 * @brief MainWindow::display_suffix_signatures
 * This is called by a QAction.
 */
void MainWindow::display_suffix_signatures()
{
    m_tableView_upper_left->setModel(m_Models["Signatures"]);
    m_tableView_upper_left->set_display_type( DT_suffixal_signatures );
    m_tableView_upper_left->resizeColumnsToContents();
    m_tableView_upper_left->set_content_type( "suffixal signatures" );

    CLexicon* lexicon = get_lexicon();
    m_tableView_upper_right->setModel(m_Models["Signatures 2"]);
    m_tableView_upper_right->set_display_type( DT_suffixal_signatures );
    m_tableView_upper_left->set_content_type( "suffixal signatures" );
    m_tableView_upper_right->resizeColumnsToContents();

    m_suffix_graphics_scene->clear_all();
    m_suffix_graphics_scene->assign_scene_positions_to_signatures(lexicon->get_signatures(), DT_All_Suffix_Signatures);
    m_suffix_graphics_scene->place_signatures();
}
/**
 * @brief MainWindow::display_prefix_signatures
 * This is called by a QAction.
 */
void MainWindow::display_prefix_signatures()
{
    m_tableView_upper_left->setModel(m_Models["Prefix signatures"]);
    m_tableView_upper_left->set_display_type ( DT_prefixal_signatures );
    m_tableView_upper_left->resizeColumnsToContents();

    m_tableView_upper_right->setModel(m_Models["Prefix signatures 2"]);
    m_tableView_upper_right->set_display_type ( DT_prefixal_signatures );
    m_tableView_upper_right->resizeColumnsToContents();

    //m_prefix_graphics_scene->clear_all();
    CLexicon* lexicon = get_lexicon();
    m_prefix_graphics_scene->assign_scene_positions_to_signatures(lexicon->get_prefix_signatures(), DT_All_Prefix_Signatures);
    m_prefix_graphics_scene->place_signatures();
}
/**
 * @brief MainWindow::display_hypotheses
 * This is called by a QAction.
 */
void MainWindow::display_hypotheses()
{    

    m_tableView_upper_left->setModel(m_Models["Hypotheses"]);
    m_tableView_upper_left->set_display_type ( DT_hypotheses );
    m_tableView_upper_left->resizeColumnsToContents();

    m_tableView_upper_right->setModel(m_Models["Hypotheses 2"]);
    m_tableView_upper_right->set_display_type ( DT_hypotheses );
    m_tableView_upper_right->resizeColumnsToContents();


}
