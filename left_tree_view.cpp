#include "mainwindow.h"
#include "Lexicon.h"
#include "compound.h"
#include "sigpaircollection.h"

void helper_function(QString label, int count, QStandardItem * lexicon_item ){
    QList<QStandardItem*>  these_items;
    QStandardItem * this_item = new QStandardItem(label);
    QStandardItem * this_item_count_item = new QStandardItem(QString::number(count));
    these_items.append(this_item);
    these_items.append(this_item_count_item);
    lexicon_item->appendRow(these_items);
}

void MainWindow::create_or_update_TreeModel(CLexicon* lexicon)
{

    QStandardItem * parent = m_treeModel->invisibleRootItem();

    // Not using these:
    QStandardItem * pos_sig_item = new QStandardItem(QString("EPositive signatures"));
    QStandardItem * pos_sig_count_item = new QStandardItem(QString::number(lexicon->get_signatures()->get_number_of_epositive_signatures()));
    QList<QStandardItem*> pos_sig_items;
    pos_sig_items.append(pos_sig_item);
    pos_sig_items.append(pos_sig_count_item);

    QStandardItem * pos_prefix_sig_item = new QStandardItem(QString("EPositive prefix signatures"));
    QStandardItem * pos_prefix_sig_count_item = new QStandardItem(QString::number(lexicon->get_prefix_signatures()->get_number_of_epositive_signatures()));
    QList<QStandardItem*> pos_prefix_sig_items;
    pos_prefix_sig_items.append(pos_prefix_sig_item);
    pos_prefix_sig_items.append(pos_prefix_sig_count_item);

    QStandardItem * residual_sig_item = new QStandardItem(QString("Residual parasignatures"));
    QStandardItem * residual_sig_count_item = new QStandardItem(QString::number(lexicon->get_residual_signatures()->get_count()));
    QList<QStandardItem*> residual_sig_items;
    residual_sig_items.append(residual_sig_item);
    residual_sig_items.append(residual_sig_count_item);

    QStandardItem * parasuffix_item = new QStandardItem(QString("Parasuffixes"));
    QStandardItem * parasuffix_count_item = new QStandardItem(QString::number(lexicon->get_parasuffixes()->get_count()));
    QList<QStandardItem*> parasuffix_items;
    parasuffix_items.append(parasuffix_item);
    parasuffix_items.append(parasuffix_count_item);

    QStandardItem * passive_signature_item = new QStandardItem(QString("Passive signatures"));
    QStandardItem * passive_signature_count_item = new QStandardItem(QString::number(lexicon->get_passive_signatures()->get_count()));
    QList<QStandardItem*> passive_signature_items;
    passive_signature_items.append(passive_signature_item);
    passive_signature_items.append(passive_signature_count_item);

    QStandardItem * hypothesis_item = new QStandardItem(QString("Hypotheses"));
    QStandardItem * hypothesis_count_item = new QStandardItem(QString::number(lexicon->get_hypotheses()->count()));
    QList<QStandardItem*> hypothesis_items;
    hypothesis_items.append(hypothesis_item);
    hypothesis_items.append(hypothesis_count_item);



 // Using these:

    QStandardItem * lexicon_item = new QStandardItem(QString("Lexicon"));
    QStandardItem * lexicon_count_item = new QStandardItem(QString("1"));
    QList<QStandardItem*> lexicon_items;
    lexicon_items.append(lexicon_item);
    lexicon_items.append(lexicon_count_item);
    parent->appendRow(lexicon_items);


    if (m_my_lexicon->get_prefixes()->get_count() > 0 ){
        helper_function("Prefixes", m_my_lexicon->get_prefixes()->get_count(), lexicon_item);
    }
    if (m_my_lexicon->get_words()->get_count() > 0){
        helper_function("Words", m_my_lexicon->get_word_collection()->get_count(), lexicon_item);
    }
    if (m_my_lexicon->get_suffixal_sig_pairs()->count()>0){
        helper_function("Suffixal sig pairs", m_my_lexicon->get_suffixal_sig_pairs()->count(), lexicon_item);
    }
    if (m_my_lexicon->get_compounds()->get_count() > 0 ){
        helper_function("Compound words", m_my_lexicon->get_compounds()->get_count(), lexicon_item);
    }
    if (m_my_lexicon->get_suffixal_protostems()->get_count() > 0 ){
    //    helper_function("Suffixal protostems", m_my_lexicon->get_suffixal_protostems()->get_count(), lexicon_item);
    }
    if (m_my_lexicon->get_prefixal_protostems()->get_count() > 0 ){
    //     helper_function("Prefixal protostems", m_my_lexicon->get_prefixal_protostems()->get_count(), lexicon_item);
    }
    if (m_my_lexicon->get_suffixal_stems()->get_count() > 0 ){
        helper_function("Suffixal stems", m_my_lexicon->get_suffixal_stems()->get_count(), lexicon_item);
    }
    if (m_my_lexicon->get_prefixal_stems()->get_count() > 0 ){
        helper_function("Prefixal stems", m_my_lexicon->get_prefixal_stems()->get_count(), lexicon_item);
    }
    if (m_my_lexicon->get_suffixes()->get_count() > 0) {
        helper_function("Suffixes", m_my_lexicon->get_suffixes()->get_count(), lexicon_item);
    }
    if (m_my_lexicon->get_suffix_signatures()->get_count() > 0){
        helper_function("Suffixal signatures", m_my_lexicon->get_signatures()->get_count(), lexicon_item);
    }
    if (m_my_lexicon->get_prefix_signatures()->get_count() > 0 ){
        helper_function("Prefix signatures", m_my_lexicon->get_prefix_signatures()->get_count(), lexicon_item);
    }
    if (m_my_lexicon->get_parses()->count() > 0 ){
        helper_function("Parses", m_my_lexicon->get_parses()->count(), lexicon_item);
    }
    if (m_my_lexicon->get_parasuffixes()->get_count() > 0 ){
        helper_function("Parasuffixes", m_my_lexicon->get_parasuffixes()->get_count(), lexicon_item);
    }

    if (m_my_lexicon->get_hypotheses()->count() > 0 )
        lexicon_item->appendRow(hypothesis_items);



    // not using:
    /*
    lexicon_item->appendRow(pos_sig_items);
    lexicon_item->appendRow(residual_sig_items);
    lexicon_item->appendRow(parasuffix_items);
    lexicon_item->appendRow(passive_signature_items);
    */
}


