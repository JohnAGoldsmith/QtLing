#include "Lexicon.h"
#include "WordCollection.h"
#include "Word.h"
#include <QProgressBar>


/*!
 * We can build a graph whose nodes are the signatures, where an edge connects
 * any pair of signatures if there exists a word that they both analyze.
 * A sig_graph_edge has two flavors: this function uses the flavor that
 * contains the two signatures, the word, and the string-difference between
 * the stems of the word at the two signatures.
 * The edge has: signature1, signature2, a morph, and a pair of stems.
 *
 * This is Part 1 of the 3rd major function of Crab 2.
 */
void CLexicon::step8a_compute_word_sig_pairs()
{
    map_string_to_word *            WordMap = m_Words->GetMap();
    map_string_to_word_ptr_iter     word_iter(*WordMap);
    word_sig_pair *                      psig_pair;
    CSignature *                    the_other_sig_ptr;
    CWord*                          pWord;
    morph_t                         difference;
    stem_t                          this_stem, the_other_stem;
    int                             analysis_number = 0;

    m_StatusBar->showMessage("5 Split morphemes: find words with multiple analyses...");
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(WordMap->size());
    int progresscount = 0;

    while (word_iter.hasNext())   {
        m_ProgressBar->setValue(progresscount++);

        pWord = word_iter.next().value();
        analysis_number = 0;
        for (int i = 0; i < pWord->get_parse_triple_list()->size(); i++)
        {   Parse_triple * this_triple = pWord->get_parse_triple_list()->at(i);
            analysis_number += 1;
            this_stem = this_triple->m_stem;
            int this_stem_length = this_stem.length();
            sigstring_t this_sig = this_triple->m_sig_string;
            CSignature* this_sig_ptr;
            m_SuffixesFlag?
                this_sig_ptr = m_Signatures->find_or_fail(this_sig):
                this_sig_ptr = m_PrefixSignatures->find_or_fail(this_sig);

            for (int j = i+1; j <pWord->get_parse_triple_list()->size(); j++ )
            {   Parse_triple* the_other_triple = pWord->get_parse_triple_list()->at(j);
                the_other_stem = the_other_triple->m_stem;
                int the_other_stem_length = the_other_stem.length();
                sigstring_t the_other_sig = the_other_triple->m_sig_string;
                m_SuffixesFlag?
                    the_other_sig_ptr = m_Signatures->find_or_fail(the_other_sig):
                    the_other_sig_ptr = m_PrefixSignatures->find_or_fail(the_other_sig);
                if (this_sig == the_other_sig){continue;}
                if (this_stem_length > the_other_stem_length ){     // this cannot happen now (it could in the previous formulation -- so fix this);
                            int length_of_difference = this_stem_length - the_other_stem_length;
                            m_SuffixesFlag?
                                difference = this_stem.mid(the_other_stem_length):
                                difference = this_stem.left(length_of_difference);
                                 psig_pair = new word_sig_pair (this, this_sig_ptr, the_other_sig_ptr, this_sig,the_other_sig,
                                                                        difference, pWord->get_key(), this_stem, the_other_stem);
                 } else{
                     int length_of_difference = the_other_stem_length - this_stem_length;
                     m_SuffixesFlag?
                           difference = the_other_stem.mid(this_stem_length):
                           difference = the_other_stem.left(length_of_difference);
                     psig_pair = new word_sig_pair(this, the_other_sig_ptr, this_sig_ptr,  the_other_sig,this_sig,
                                                                difference, pWord->get_key(), the_other_stem, this_stem);
                }
                m_SigPairList.append(psig_pair);
                QString message1 = "sig graph edge #" + QString::number(analysis_number) + "=signature 1: ="
                        + this_stem + "=" +  this_sig + "=" + "difference =" + "="  + difference;
                add_to_word_autobiographies(pWord->get_key(), message1);
                QString message2 = "=signature 2:   =" + the_other_stem + "=" + the_other_sig;
                add_to_word_autobiographies(pWord->get_key(),message2);
            } // end of looking at stems longer than this_stem
        } //end of looking at each stem in this word.
    } // each word
}

/*!
 * This function takes the sig-pair list, and makes a smaller list composed of
 * sig_pair which share the same signature pair and string-difference. This
 * flavor of sig_graph_edge contains a list of all the words that participate in
 * this particular sig_graph_edge.
 *
 * This is Part 2 of the 3rd major function of Crab 2.
 */
void CLexicon::step8b_compute_sig_pair_map() {
    morph_t         edge_label;
    word_t          this_word;
    word_sig_pair   * p_word_sig_pair;
    sig_pair        * p_sig_pair;

    m_StatusBar->showMessage("8b: Generating map of signature graph edges");
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(m_SigPairList.size());
    int progresscount = 0;

    for (int i = 0; i < m_SigPairList.size(); i++)
    {   m_ProgressBar->setValue(progresscount++);
        p_word_sig_pair = m_SigPairList[i];
        edge_label = p_word_sig_pair->label();
        this_word  = p_word_sig_pair->word;
        if (m_SigPairMap.contains(edge_label)){
            p_sig_pair = m_SigPairMap.value(edge_label);
            word_stem_struct * this_word_stem_struct = new word_stem_struct;
            this_word_stem_struct->word = this_word;
            this_word_stem_struct->stem_1 = p_word_sig_pair->stem_1;
            this_word_stem_struct->stem_2 = p_word_sig_pair->stem_2;
            QString this_label = this_word_stem_struct->get_label();
            if ( ! p_sig_pair->shared_word_stems.contains(this_label)){
                   p_sig_pair->shared_word_stems[this_label] = this_word_stem_struct;
            }
        } else {
            p_sig_pair = new sig_pair(*p_word_sig_pair);
            m_SigPairMap.insert(p_sig_pair->label(),p_sig_pair);
        }
    }
}
