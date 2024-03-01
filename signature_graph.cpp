#include "Lexicon.h"
#include "WordCollection.h"
#include "Word.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "Lexicon.h"
#include <QProgressBar>
#include <hypothesis.h>
#include "sigpair.h"
#include "sigpaircollection.h"

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
// helper function:
void add_label_to_sig_pairs(sig_pair_map * SigPairMap,
                            QString label,
                            QString shorter_stem,
                            QString shorter_stem_sig_string,
                            QString longer_stem_sig_string,
                            QString difference) {
    sig_pair * p_sig_pair;
    if (SigPairMap->contains(label)){
        p_sig_pair = (*SigPairMap)[label];
        if (p_sig_pair->contains_stem(shorter_stem)){
          return;
        }
        p_sig_pair->add_stem(shorter_stem);
    }else{
        p_sig_pair = new sig_pair(difference, shorter_stem_sig_string, longer_stem_sig_string);
        p_sig_pair->add_stem(shorter_stem);
        SigPairMap->insert(label, p_sig_pair);
    }
}
void CLexicon::add_to_word_biographies(int analysis_number,
                             QString shorter_stem,
                             QString shorter_stem_sig_string,
                             QString difference,
                             CWord * pWord,
                             QString longer_stem,
                             QString longer_stem_sig_string){
    QString message1 = "Comparison #" + QString::number(analysis_number) + "=signature 1:="
                       + shorter_stem + "=" +  shorter_stem_sig_string + "=" + "difference" + "="  + difference;
    add_to_word_autobiographies(pWord->get_key(), message1);
    QString message2 = "=signature 2:   =" + longer_stem + "=" + longer_stem_sig_string;
    add_to_word_autobiographies(pWord->get_key(),message2);
}
void CLexicon::step8a_compute_word_sig_pairs()
{

    CSignature *    shorter_stem_sig_ptr, * longer_stem_sig_ptr;
    CWord*          pWord;
    morph_t         difference;
    int             analysis_number = 0, length_of_difference;
    word_t          this_word;
    sig_pair        * p_sig_pair;
    QString         shorter_stem, longer_stem;
    QString         shorter_stem_sig_string, longer_stem_sig_string, label;
    m_StatusBar->showMessage("5 Split morphemes: find words with multiple analyses...");
    initialize_progress(m_Words->get_count());
    for (int i = 0; i < m_Words->get_word_count(); i++){
        mark_progress(i);
        pWord = m_Words->get_word(i);
        this_word = pWord->get_key();
        analysis_number = 0;
        for (int i = 0; i < pWord->get_parse_triple_list()->size(); i++)
        {   Parse_triple * shorter_stem_triple = pWord->get_parse_triple_list()->at(i);
            shorter_stem = shorter_stem_triple->m_stem;
            for (int j = i+1; j <pWord->get_parse_triple_list()->size(); j++ )
            {   Parse_triple* longer_stem_triple = pWord->get_parse_triple_list()->at(j);
                analysis_number += 1;
                longer_stem = longer_stem_triple->m_stem;
                length_of_difference = longer_stem.length() - shorter_stem.length();
                if (m_suffix_flag)
                {
                    shorter_stem_sig_ptr = m_Signatures->find_or_fail(shorter_stem_triple->m_sig_string);
                    longer_stem_sig_ptr = m_Signatures->find_or_fail(longer_stem_triple->m_sig_string);
                    difference = longer_stem.right(length_of_difference);
                }else{
                    shorter_stem_sig_ptr = m_PrefixSignatures->find_or_fail(shorter_stem_triple->m_sig_string);
                    longer_stem_sig_ptr = m_PrefixSignatures->find_or_fail(longer_stem_triple->m_sig_string);
                    difference = longer_stem.left(length_of_difference);
                }
                if (! shorter_stem_sig_ptr ) {
                    qDebug() << 91 << "signature not found: " << shorter_stem_triple->m_sig_string;
                }
                if (! longer_stem_sig_ptr ) {
                    qDebug() << 95 << "signature not found: " << longer_stem_triple->m_sig_string;
                }

                shorter_stem_sig_string = shorter_stem_sig_ptr->display();
                longer_stem_sig_string = longer_stem_sig_ptr->display();
                if ( shorter_stem_sig_ptr == longer_stem_sig_ptr){
                    continue;}
                else {
                    label = difference + "/" + shorter_stem_sig_string + "/" + longer_stem_sig_string;
                }
                add_label_to_sig_pairs(m_suffixal_sig_pairs->get_map(), label, shorter_stem, shorter_stem_sig_string,
                                       longer_stem_sig_string, difference);
                add_to_word_biographies(analysis_number, shorter_stem, shorter_stem_sig_string,
                                        difference, pWord, longer_stem, longer_stem_sig_string);
                //qDebug() << "\n" << this_word<< i << j << analysis_number << "\n" << label << "\n" << message1 << "\n" << message2;
            } // end of looking at stems longer than this_stem
        } //end of looking at each stem in this word.
    } // each word
}


QString F2(QString affix, QString morph, bool suffix_flag){
    QString doomed_affix;
    if (affix == "NULL"){
        doomed_affix = morph;
    } else{
        suffix_flag?
           doomed_affix = morph + affix:
           doomed_affix = affix + morph;
    }
 return doomed_affix;
}
QStringList F3(QStringList affixes, QString morph, bool suffix_flag){
    QStringList new_affix_list;
    foreach (QString affix, affixes){
        if (affix == "NULL"){
            new_affix_list.append(morph);
        } else{
            suffix_flag?
               new_affix_list.append(morph + affix):
               new_affix_list.append(affix + morph);
        }
    }
    return new_affix_list;
}
void remove_parses_1 (CLexicon* lexicon,    QStringList& stems, QStringList&  affixes, QString difference){
    foreach (QString stem, stems ){
           QString longer_stem = stem + difference;
           foreach(QString suffix, affixes){
               CParse this_parse(longer_stem, suffix, lexicon->get_suffix_flag());
               lexicon->remove_parse(& this_parse);
           }
    }
}
void remove_parses_2(CLexicon* lexicon, QStringList& stems, QStringList& affixes, QString difference){
    foreach(QString stem, stems ){
       foreach(QString suffix, affixes ){
             if (suffix.startsWith(difference)){
                    CParse this_parse(stem, suffix, lexicon->get_suffix_flag());
                    lexicon->remove_parse(this_parse);
             }
       }
    }
}
// add suffixes to long-stem signature if necessary
void add_suffixes_to_signature_stringlist(QString difference,
                                          QStringList affixes_from_shorter_stem,
                                          QStringList affixes_from_longer_stem){
    int difference_length = difference.length();
    foreach (QString this_affix, affixes_from_shorter_stem){
        if (this_affix.startsWith(difference)){
            QString remnant  = this_affix.mid(difference_length);
            if (remnant.length() == 0) { remnant= "NULL";}
            if (! affixes_from_longer_stem.contains(remnant)) {
                affixes_from_longer_stem.append(remnant);
            }
        }
    }
}
void CLexicon::step8c_from_sig_pairs_to_parses_Create_hypotheses(){
    sig_pair_iter sig_pair_iter (*m_suffixal_sig_pairs->get_map());
    QString     this_affix, doomed_affix, remnant, difference;
    QString shorter_stem, longer_stem;
    QString shorter_stem_sig_string, longer_stem_sig_string;
    QStringList affected_signatures;
    QString extended_difference_name;
    sig_pair*   p_sig_pair;
    int MINIMUM_NUMBER_OF_WORDS = M_MINIMUM_HYPOTHESIS_WORD_COUNT; // usually 2
    int difference_length;
    while (sig_pair_iter.hasNext())
    {
         p_sig_pair = sig_pair_iter.next().value();

         QString difference = p_sig_pair->get_morph();
         difference_length = difference.length();
         if (difference_length < 2){ continue;}
         if (p_sig_pair->get_number_of_words() < MINIMUM_NUMBER_OF_WORDS ){ continue;}

         QStringList affixes_from_shorter_stem = p_sig_pair->get_sig1_string().split("=");
         QStringList affixes_from_longer_stem  = p_sig_pair->get_sig2_string().split("=");
         
         if (m_suffix_flag){

             int count = get_internal_affix_count(difference, affixes_from_longer_stem);
             extended_difference_name = difference + ":" + QString::number(count);

             // remove parses of longer-stem + affixes in long-stem signature.
             remove_parses_1 (this, p_sig_pair->get_my_stems(), affixes_from_longer_stem, difference);

             // add suffixes to long-stem signature if necessary
             add_suffixes_to_signature_stringlist(difference, affixes_from_shorter_stem, affixes_from_longer_stem);

             // remove parses made from short-stem plus suffixes that start with difference
             remove_parses_2(this, p_sig_pair->get_my_stems(), affixes_from_shorter_stem, difference);

             // add a parse of difference (=stem) + each of the suffixes of the long-stem signature.
             foreach (QString suffix, affixes_from_longer_stem){
                 CParse * pParse = new CParse(extended_difference_name, suffix, m_suffix_flag);
                 add_parse(pParse);
             }
             // add a parse of shorter-stem plus difference
             foreach (QString stem, p_sig_pair->get_my_stems()){
                 CParse * pParse = new CParse(stem, extended_difference_name, m_suffix_flag);
                 add_parse(pParse);
             }
         }
         affixes_from_longer_stem.sort();
         longer_stem_sig_string = affixes_from_longer_stem.join("=");
         CHypothesis * this_hypothesis = new CHypothesis( HT_affix_goes_to_signature,
                                                           extended_difference_name,
                                                           longer_stem_sig_string,
                                                           p_sig_pair->get_sig1_string(),
                                                           p_sig_pair->get_number_of_words());
         m_Hypotheses->append(this_hypothesis);
         m_Hypothesis_map->insert (this_hypothesis->express_as_string(),  this_hypothesis);
    }
    //  !! Note: here we want to allow stems that only occur once -- they are pre-approved, so to speak.
    step3_from_parses_to_stem2sig_maps("Splitting up complex morphemes");
    step4_create_signatures("Splitting up complex morphemes.");
}















