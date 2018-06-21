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
        m_tableView_upper_left->set_data_type ( e_data_suffixes );
        m_tableView_upper_left->resizeColumnsToContents();
     }

    /**
     * @brief MainWindow::display_epositive_suffix_signatures
     */
    void MainWindow::display_epositive_suffix_signatures(CLexicon* lexicon)
    {
        if (m_Models["EPositive signatures"]->rowCount() > 0){
            m_tableView_upper_left->setModel(m_Models["EPositive signatures"]);
            m_tableView_upper_left->set_data_type(e_data_epositive_suffixal_signatures );

            m_tableView_upper_right->setModel(m_Models["EPositive signatures 2"]);
            m_tableView_upper_right->set_data_type( e_data_epositive_suffixal_signatures );
            m_tableView_upper_right->resizeColumnsToContents();

            m_graphics_scene->assign_lattice_positions_to_signatures(get_lexicon()->get_signatures(), e_data_epositive_suffixal_signatures);
            m_graphics_scene->create_and_place_signatures();
        }
    }
    void MainWindow::display_epositive_prefix_signatures(CLexicon* lexicon)
    {
           if (m_Models["EPositive prefix signatures"]->rowCount() > 0){
            m_tableView_upper_left->setModel(m_Models["EPositive prefix signatures"]);
            m_tableView_upper_left->set_data_type(e_data_epositive_prefixal_signatures );
            m_tableView_upper_left->set_data_type(e_data_epositive_prefixal_signatures);

            m_tableView_upper_right->setModel(m_Models["EPositive prefix signatures 2"]);
            m_tableView_upper_right->set_data_type( e_data_epositive_prefixal_signatures );
            m_tableView_upper_right->resizeColumnsToContents();

            m_graphics_scene->clear_all();
            m_graphics_scene->assign_lattice_positions_to_signatures(lexicon->get_prefix_signatures(), e_data_epositive_suffixal_signatures);
            m_graphics_scene->create_and_place_signatures();
        }
    }

    /**
     * @brief MainWindow::display_suffix_signatures
     * Thi  s is called by a QAction.
     */
    void MainWindow::display_suffix_signatures(CLexicon* lexicon)
    {

        m_tableView_upper_left->setModel(m_Models["Signatures"]);
        m_tableView_upper_left->set_data_type( e_data_suffixal_signatures );
        m_tableView_upper_left->resizeColumnsToContents();

        m_tableView_upper_right->setModel(m_Models["Signatures 2"]);
        m_tableView_upper_right->set_data_type( e_data_suffixal_signatures );
        m_tableView_upper_right->resizeColumnsToContents();

         m_graphics_scene->clear_all();
         m_graphics_scene->assign_lattice_positions_to_signatures(lexicon->get_signatures(), e_data_suffixal_signatures);
         m_graphics_scene->create_and_place_signatures();




    }



    /**
     * @brief MainWindow::display_prefix_signatures
     * This is called by a QAction.
     */
    void MainWindow::display_prefix_signatures(CLexicon* lexicon)
    {
        m_tableView_upper_left->setModel(m_Models["Prefix signatures"]);
        m_tableView_upper_left->set_data_type ( e_data_prefixal_signatures );
        m_tableView_upper_left->resizeColumnsToContents();

        m_tableView_upper_right->setModel(m_Models["Prefix signatures 2"]);
        m_tableView_upper_right->set_data_type ( e_data_prefixal_signatures );
        m_tableView_upper_right->resizeColumnsToContents();

        m_graphics_scene->clear_all();
        m_graphics_scene->assign_lattice_positions_to_signatures(lexicon->get_prefix_signatures(), e_data_prefixal_signatures);
        m_graphics_scene->create_and_place_signatures();
    }

    void MainWindow::display_signature_graph_edges(CLexicon* lexicon){
        m_tableView_upper_left->setModel(m_Models["SigGraphEdges_1"]);
        m_tableView_upper_left->set_data_type( e_data_signatures_graph_edges );
        m_tableView_upper_left->resizeColumnsToContents();

        m_tableView_upper_right->setModel(m_Models["SigGraphEdges_2"]);
        m_tableView_upper_right->set_data_type( e_data_signatures_graph_edges );
        m_tableView_upper_right->resizeColumnsToContents();



    }


    /**
     * @brief MainWindow::display_hypotheses
     * This is called by a QAction.
     */
    void MainWindow::display_hypotheses()
    {

        m_tableView_upper_left->setModel(m_Models["Hypotheses"]);
        m_tableView_upper_left->set_data_type ( e_data_hypotheses );
        m_tableView_upper_left->resizeColumnsToContents();

        m_tableView_upper_right->setModel(m_Models["Hypotheses 2"]);
        m_tableView_upper_right->set_data_type ( e_data_hypotheses );
        m_tableView_upper_right->resizeColumnsToContents();


    }


