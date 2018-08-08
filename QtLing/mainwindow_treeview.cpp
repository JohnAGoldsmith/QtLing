/* I moved any functions related to the treeview in the main window to here
 * because more and more things got added to the treeview and lxamodels.cpp
 * was getting too long.
 *
 * Comment added by Hanson 8.1
 */

#include "mainwindow.h"
#include "WordCollection.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "compound.h"

#include <QDebug>

void remove_item_from_tree(const QString name, QStandardItem* item)
{
    for (int row_i = 0; row_i < item->rowCount(); ) {
        QString data = item->child(row_i, 0)->data(Qt::DisplayRole).toString();
        if (data == name) {
            item->removeRow(row_i);
        } else {
            row_i++;
        }
    }
}

void MainWindow::append_eval_results(EvaluationResults& results, QStandardItem* parent_item)
{
    typedef QStandardItem QSI;
    QList<QSI*> word_items;
    QSI* word_item = new QSI(QString("Gold Standard Words"));
    int gs_word_count = results.get_gs_word_count();
    QSI* word_count_item = new QSI(QString::number(gs_word_count));
    word_items.append(word_item);
    word_items.append(word_count_item);

    QList<QSI*> retrieved_word_items;
    QSI* retrieved_word_item = new QSI(QString("Retrieved Words"));
    int retrieved_word_count = results.get_retrieved_word_count();
    QSI* retrieved_word_count_item = new QSI(QString::number(retrieved_word_count));
    retrieved_word_items.append(retrieved_word_item);
    retrieved_word_items.append(retrieved_word_count_item);

    QList<QSI*> precision_items;
    QSI* precision_item = new QSI(QString("Precision"));
    double precision = results.get_total_precision();
    QSI* precision_value_item = new QSI(QString::number(precision));
    precision_items.append(precision_item);
    precision_items.append(precision_value_item);

    QList<QSI*> recall_items;
    QSI* recall_item = new QSI(QString("Recall"));
    double recall = results.get_total_recall();
    QSI* recall_value_item = new QSI(QString::number(recall));
    recall_items.append(recall_item);
    recall_items.append(recall_value_item);

    QList<QSI*> true_positive_items;
    QSI* true_positive_item = new QSI(QString("True Positive Parses"));
    int true_positive_count = results.get_true_positive_count();
    QSI* true_positive_count_item = new QSI(QString::number(true_positive_count));
    true_positive_items.append(true_positive_item);
    true_positive_items.append(true_positive_count_item);

    QList<QSI*> correct_items;
    QSI* correct_item = new QSI(QString("Gold Standard Parses"));
    int correct_count = results.get_gs_parse_count();
    QSI* correct_count_item = new QSI(QString::number(correct_count));
    correct_items.append(correct_item);
    correct_items.append(correct_count_item);

    QList<QSI*> retrieved_items;
    QSI* retrieved_item = new QSI(QString("Retrieved Parses"));
    int retrieved_count = results.get_retrieved_parse_count();
    QSI* retrieved_count_item = new QSI(QString::number(retrieved_count));
    retrieved_items.append(retrieved_item);
    retrieved_items.append(retrieved_count_item);

    parent_item->appendRow(word_items);
    parent_item->appendRow(retrieved_word_items);
    parent_item->appendRow(precision_items);
    parent_item->appendRow(recall_items);
    parent_item->appendRow(true_positive_items);
    parent_item->appendRow(correct_items);
    parent_item->appendRow(retrieved_items);
}

void MainWindow::update_TreeModel_for_gs(CLexicon* lexicon)
{
    typedef QStandardItem QSI;
    QSI* parent = m_treeModel->invisibleRootItem();
    int parent_row_count = parent->rowCount();
    QSI* curr_lexicon_item = parent->child(parent_row_count-1, 0);

    remove_item_from_tree("Gold Standard", curr_lexicon_item);

    QSI* gs_item = new QSI(QString("Gold Standard"));

    append_eval_results(lexicon->get_goldstandard()->get_results(), gs_item);

    curr_lexicon_item->appendRow(gs_item);
}

void MainWindow::update_TreeModel_for_eval(CLexicon *lexicon)
{
    if (lexicon->get_eval_parses() == NULL) {
        qDebug() << "MainWindow::update_TreeModel_for_eval: "
                    "EvalParses not loaded";
        return;
    }

    typedef QStandardItem QSI;
    QSI* parent = m_treeModel->invisibleRootItem();
    int parent_row_count = parent->rowCount();
    QSI* curr_lexicon_item = parent->child(parent_row_count-1, 0);

    QSI* eval_item = new QSI(QString("Morfessor Parses"));

    append_eval_results(lexicon->get_eval_parses()->get_results(), eval_item);

    curr_lexicon_item->appendRow(eval_item);
}

void MainWindow::create_or_update_TreeModel(CLexicon* lexicon)
{
    // possible take them out
     QStandardItem * parent = m_treeModel->invisibleRootItem();

     QStandardItem * command_item = new QStandardItem(QString("Keyboard commands"));

     QStandardItem * ctrl_1 = new QStandardItem(QString("Prefixes step 2"));
     QStandardItem * ctrl_1_key = new QStandardItem("Ctrl 1");

     QStandardItem * ctrl_2 = new QStandardItem(QString("Suffixes step 2"));
     QStandardItem * ctrl_2_key = new QStandardItem("Ctrl 2");

     QStandardItem * ctrl_3 = new QStandardItem(QString("Read project file"));
     QStandardItem * ctrl_3_key = new QStandardItem("Ctrl 3");

     QStandardItem * ctrl_4 = new QStandardItem(QString("Read corpus"));
     QStandardItem * ctrl_4_key = new QStandardItem("Ctrl 4");

     QStandardItem * ctrl_5 = new QStandardItem(QString("Sublexicon"));
     QStandardItem * ctrl_5_key = new QStandardItem("Ctrl 5");



    //  this pair of lines must stay here after experiment:
    QStandardItem * lexicon_item = new QStandardItem(QString("Lexicon"));
    QStandardItem * lexicon_count_item = new QStandardItem(QString("1"));

    QStandardItem * suffix_flag_item;
    if (lexicon->get_suffix_flag()){
        suffix_flag_item = new QStandardItem(QString("Suffixes"));
    } else {
        suffix_flag_item = new QStandardItem(QString("Prefixes"));
    }

    // will be eliminated by the experiment:
    QStandardItem * word_item = new QStandardItem(QString("Words"));
    QStandardItem * word_count_item = new QStandardItem(QString::number(lexicon->get_word_collection()->get_count()));

    QStandardItem * compound_item = new QStandardItem(QString("Compound words"));
    QStandardItem * compound_count_item = new QStandardItem(QString::number(lexicon->get_compounds()->get_count()));

    QStandardItem * suffixal_protostem_item = new QStandardItem(QString("Suffixal protostems"));
    QStandardItem * suffixal_protostem_count_item = new QStandardItem(QString::number(lexicon->get_suffixal_protostems()->size()));

    QStandardItem * prefixal_protostem_item = new QStandardItem(QString("Prefixal protostems"));
    QStandardItem * prefixal_protostem_count_item = new QStandardItem(QString::number(lexicon->get_prefixal_protostems()->size()));

    QStandardItem * suffixal_stem_item = new QStandardItem(QString("Suffixal stems"));
    QStandardItem * suffixal_stem_count_item = new QStandardItem(QString::number(lexicon->get_suffixal_stems()->get_count()));

    QStandardItem * prefixal_stem_item = new QStandardItem(QString("Prefixal stems"));
    QStandardItem * prefixal_stem_count_item = new QStandardItem(QString::number(lexicon->get_prefixal_stems()->get_count()));


    QStandardItem * suffix_item = new QStandardItem(QString("Suffixes"));
    QStandardItem * suffix_count_item = new QStandardItem(QString::number(lexicon->get_suffixes()->get_count()));

    QStandardItem * prefix_item = new QStandardItem(QString("Prefixes"));
    QStandardItem * prefix_count_item = new QStandardItem(QString::number(lexicon->get_prefixes()->get_count()));


    QStandardItem * sig_item = new QStandardItem(QString("Signatures"));
    QStandardItem * sig_count_item = new QStandardItem(QString::number(lexicon->get_signatures()->get_count()));

    QStandardItem * pos_sig_item = new QStandardItem(QString("EPositive signatures"));
    QStandardItem * pos_sig_count_item = new QStandardItem(QString::number(lexicon->get_signatures()->get_number_of_epositive_signatures()));

    QStandardItem * prefix_sig_item = new QStandardItem(QString("Prefix signatures"));
    QStandardItem * prefix_sig_count_item = new QStandardItem(QString::number(lexicon->get_prefix_signatures()->get_count()));

    QStandardItem * pos_prefix_sig_item = new QStandardItem(QString("EPositive prefix signatures"));
    QStandardItem * pos_prefix_sig_count_item = new QStandardItem(QString::number(lexicon->get_prefix_signatures()->get_number_of_epositive_signatures()));

    QStandardItem * residual_sig_item = new QStandardItem(QString("Residual parasignatures"));
    QStandardItem * residual_sig_count_item = new QStandardItem(QString::number(lexicon->get_residual_signatures()->get_count()));

    QStandardItem * parasuffix_item = new QStandardItem(QString("Parasuffixes"));
    QStandardItem * parasuffix_count_item = new QStandardItem(QString::number(lexicon->get_parasuffixes()->get_count()));

    QStandardItem * sig_graph_edge_item = new QStandardItem(QString("Signature graph edges"));
    QStandardItem * sig_graph_edge_count_item = new QStandardItem(QString::number(lexicon->get_sig_graph_edge_map()->size()));

    QStandardItem * passive_signature_item = new QStandardItem(QString("Passive signatures"));
    QStandardItem * passive_signature_count_item = new QStandardItem(QString::number(lexicon->get_passive_signatures()->get_count()));

    QStandardItem * hypothesis_item = new QStandardItem(QString("Hypotheses"));
    QStandardItem * hypothesis_count_item = new QStandardItem(QString::number(lexicon->get_hypotheses()->count()));

// This is part of an experiment:
//  This code deals with the components in the Lexicon, so that that set can be easily updated by the programmer.
//  it will eliminate above code and below code too:
//
//          QList<QStandardItem*>                   lexicon_items;
//                                                  lexicon_items.append(lexicon_item);
//                                                  lexicon_items.append(lexicon_count_item);
    //                                              parent->appendRow(lexicon_items);
    //      QMapIterator<QString,eComponentType>    iter (lexicon->get_category_types());
    //      while (iter.hasNext()){
    //
    //          QString             component_name = iter.next().key();
    //          eComponentType      this_component_type = iter.value();
    //          QList<QStandardItem*> this_list_of_standard_items;
    //
    //          QStandardItem *     component_item = new QStandardItem(component_name);
    //          QStandardItem *     component_count_item = new QStandardItem(component_count_name);
    //            this_list_of_standard_items.append(component_item);
    //            this_list_of_standard_items.append(component_count_item);
    //            lexicon_item.appendRow(this_list_of_standard_items);
    //    }
    // end of experiment




// add component 6



    QList<QStandardItem*> keyboard_1;
    keyboard_1.append(ctrl_1);
    keyboard_1.append(ctrl_1_key);

    QList<QStandardItem*> keyboard_2;
    keyboard_2.append(ctrl_2);
    keyboard_2.append(ctrl_2_key);

    QList<QStandardItem*> keyboard_3;
    keyboard_3.append(ctrl_3);
    keyboard_3.append(ctrl_3_key);

    QList<QStandardItem*> keyboard_4;
    keyboard_4.append(ctrl_4);
    keyboard_4.append(ctrl_4_key);

    QList<QStandardItem*> keyboard_5;
    keyboard_5.append(ctrl_5);
    keyboard_5.append(ctrl_5_key);



    QList<QStandardItem*> lexicon_items;
    lexicon_items.append(lexicon_item);
    lexicon_items.append(lexicon_count_item);

    QList<QStandardItem*> word_items;
    word_items.append(word_item);
    word_items.append(word_count_item);

    // Displaying compound words
    QList<QStandardItem*> compound_items;
    compound_items.append(compound_item);
    compound_items.append(compound_count_item);

    // Displaying Protostems
    QList<QStandardItem*> suffixal_protostem_items;
    suffixal_protostem_items.append(suffixal_protostem_item);
    suffixal_protostem_items.append(suffixal_protostem_count_item);

    QList<QStandardItem*> prefixal_protostem_items;
    prefixal_protostem_items.append(prefixal_protostem_item);
    prefixal_protostem_items.append(prefixal_protostem_count_item);

    QList<QStandardItem*> suffixal_stem_items;
    suffixal_stem_items.append(suffixal_stem_item);
    suffixal_stem_items.append(suffixal_stem_count_item);

    QList<QStandardItem*> prefixal_stem_items;
    prefixal_stem_items.append(prefixal_stem_item);
    prefixal_stem_items.append(prefixal_stem_count_item);

    QList<QStandardItem*> prefix_items;
    prefix_items.append(prefix_item);
    prefix_items.append(prefix_count_item);

    QList<QStandardItem*> suffix_items;
    suffix_items.append(suffix_item);
    suffix_items.append(suffix_count_item);

    QList<QStandardItem*> sig_items;
    sig_items.append(sig_item);
    sig_items.append(sig_count_item);

    QList<QStandardItem*> pos_sig_items;
    pos_sig_items.append(pos_sig_item);
    pos_sig_items.append(pos_sig_count_item);

    QList<QStandardItem*> prefix_sig_items;
    prefix_sig_items.append(prefix_sig_item);
    prefix_sig_items.append(prefix_sig_count_item);

    QList<QStandardItem*> pos_prefix_sig_items;
    pos_prefix_sig_items.append(pos_prefix_sig_item);
    pos_prefix_sig_items.append(pos_prefix_sig_count_item);

    QList<QStandardItem*> residual_sig_items;
    residual_sig_items.append(residual_sig_item);
    residual_sig_items.append(residual_sig_count_item);

    QList<QStandardItem*> parasuffix_items;
    parasuffix_items.append(parasuffix_item);
    parasuffix_items.append(parasuffix_count_item);

    QList<QStandardItem*> sig_graph_edge_items;
    sig_graph_edge_items.append(sig_graph_edge_item);
    sig_graph_edge_items.append(sig_graph_edge_count_item);

    QList<QStandardItem*> passive_signature_items;
    passive_signature_items.append(passive_signature_item);
    passive_signature_items.append(passive_signature_count_item);



    QList<QStandardItem*> hypothesis_items;
    hypothesis_items.append(hypothesis_item);
    hypothesis_items.append(hypothesis_count_item);

// add component 7

    parent->appendRow(lexicon_items);
    lexicon_item->appendRow(keyboard_1);
    lexicon_item->appendRow(keyboard_2);
    lexicon_item->appendRow(keyboard_3);
    lexicon_item->appendRow(keyboard_4);
    lexicon_item->appendRow(keyboard_5);
    lexicon_item->appendRow(prefix_items);
    lexicon_item->appendRow(word_items);
    lexicon_item->appendRow(compound_items);
    lexicon_item->appendRow(suffixal_protostem_items);
    lexicon_item->appendRow(prefixal_protostem_items);
    lexicon_item->appendRow(suffixal_stem_items);
    lexicon_item->appendRow(prefixal_stem_items);
    lexicon_item->appendRow(suffix_items);
    lexicon_item->appendRow(sig_items);
    lexicon_item->appendRow(pos_sig_items);
    lexicon_item->appendRow(prefix_sig_items);
    lexicon_item->appendRow(pos_prefix_sig_items);
    lexicon_item->appendRow(sig_graph_edge_items);
    lexicon_item->appendRow(residual_sig_items);
    lexicon_item->appendRow(parasuffix_items);
    lexicon_item->appendRow(passive_signature_items);
    lexicon_item->appendRow(hypothesis_items);
// add component 8
}
