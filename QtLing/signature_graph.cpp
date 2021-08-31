#include "Lexicon.h"
#include "WordCollection.h"
#include "Word.h"
#include <QProgressBar>
#include <hypothesis.h>

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
    word_sig_pair *                      p_word_sig_pair;
    CSignature *                    the_other_sig_ptr;
    CWord*                          pWord;
    morph_t                         difference;
    stem_t                          this_stem, the_other_stem;
    int                             analysis_number = 0;
    morph_t         word_sig_pair_label;
    word_t          this_word;
    sig_pair        * p_sig_pair;



    m_StatusBar->showMessage("5 Split morphemes: find words with multiple analyses...");
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(WordMap->size());
    int progresscount = 0;

    while (word_iter.hasNext())   {
        m_ProgressBar->setValue(progresscount++);

        pWord = word_iter.next().value();
        this_word = pWord->get_key();
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
                if (this_stem_length > the_other_stem_length ){
                            int length_of_difference = this_stem_length - the_other_stem_length;
                            //qDebug() << 67<<  length_of_difference;
                            m_SuffixesFlag?
                                difference = this_stem.mid(the_other_stem_length):
                                difference = this_stem.left(length_of_difference);
                                p_word_sig_pair = new word_sig_pair (this, this_sig_ptr, the_other_sig_ptr, this_sig,the_other_sig,
                                                                        difference, pWord->get_key(), this_stem, the_other_stem);
                                //qDebug() << 73 << difference << this_stem << the_other_stem;

                 } else{
                     int length_of_difference = the_other_stem_length - this_stem_length;
                     m_SuffixesFlag?
                           difference = the_other_stem.mid(this_stem_length):
                           difference = the_other_stem.left(length_of_difference);
                     p_word_sig_pair = new word_sig_pair(this, the_other_sig_ptr, this_sig_ptr,  the_other_sig,this_sig,
                                                                difference, this_word, the_other_stem, this_stem);
                }
                m_WordSigPairList.append(p_word_sig_pair);
                word_sig_pair_label = p_word_sig_pair->label();
                //qDebug() << 84 << word_sig_pair_label;
                if (m_SigPairMap.contains(word_sig_pair_label)){
                    p_sig_pair = m_SigPairMap.value(word_sig_pair_label);
                    word_stem_struct * this_word_stem_struct = new word_stem_struct(
                                this_word,
                                p_word_sig_pair->longer_stem,
                                p_word_sig_pair->shorter_stem
                                );
                    //qDebug() << 93 << " longer stem first " << p_word_sig_pair->longer_stem << p_word_sig_pair ->shorter_stem;
                    QString this_label = this_word_stem_struct->get_label();
                    if ( ! p_sig_pair->shared_word_stems.contains(this_label)){
                           p_sig_pair->shared_word_stems[this_label] = this_word_stem_struct;
                    }
                } else {
                    p_sig_pair = new sig_pair(*p_word_sig_pair);
                    //qDebug() << 100 <<  p_word_sig_pair->morph << "longer stem first:" << p_word_sig_pair->longer_stem << p_word_sig_pair->shorter_stem;
                    m_SigPairMap.insert(p_sig_pair->label(),p_sig_pair);
                }

                QString message1 = "sig graph edge #" + QString::number(analysis_number) + "=signature 1: ="
                        + this_stem + "=" +  this_sig + "=" + "difference =" + "="  + difference;
                add_to_word_autobiographies(pWord->get_key(), message1);
                QString message2 = "=signature 2:   =" + the_other_stem + "=" + the_other_sig;
                add_to_word_autobiographies(pWord->get_key(),message2);
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
void CLexicon::step8c_from_sig_pairs_to_parses(){
    sig_pair_iter sig_pair_iter (m_SigPairMap);
    //CSignatureCollection * p_signatures;
    //m_SuffixesFlag ?
    //    p_signatures = m_Signatures:
    //    p_signatures = m_PrefixSignatures;
    QString     this_affix, doomed_affix, morph_piece;
    QStringList affixes_from_shorter_stem, affixes_from_longer_stem, doomed_affixes;
    QStringList affected_signatures;
    CSignature* pSig1_longer_stem;
    CSignature* pSig2_shorter_stem;
    sig_pair*   p_sig_pair;
    int MINIMUM_NUMBER_OF_WORDS = M_MINIMUM_HYPOTHESIS_WORD_COUNT;
    int morph_length;
    while (sig_pair_iter.hasNext())
    {
         p_sig_pair = sig_pair_iter.next().value();

         QString this_morph = p_sig_pair->get_morph();
         morph_length = this_morph.length();
         if (morph_length < 2){
             continue;
         }

         pSig1_longer_stem = p_sig_pair->get_sig_1();
         pSig2_shorter_stem = p_sig_pair->get_sig_2();

         if (pSig1_longer_stem ->get_stem_entropy() < m_entropy_threshold_for_stems ||
             pSig2_shorter_stem->get_stem_entropy() < m_entropy_threshold_for_stems  ){
             continue;
         }
         if (p_sig_pair->get_number_of_words() < MINIMUM_NUMBER_OF_WORDS ){continue;}

         doomed_affixes.clear();
         pSig1_longer_stem->get_affix_string_list(affixes_from_longer_stem);
         pSig2_shorter_stem->get_affix_string_list(affixes_from_shorter_stem);

         qDebug() << 230 <<  p_sig_pair->m_sig_string_2 <<  p_sig_pair->m_sig_string_1;
         for (int affixno = 0; affixno < affixes_from_longer_stem.count(); affixno++){
            if (m_SuffixesFlag){
                foreach(word_stem_struct * this_word_stem_struct, p_sig_pair->shared_word_stems ){
                    foreach(QString suffix, affixes_from_longer_stem){

                        CParse this_parse(this_word_stem_struct->longer_stem,
                                              suffix,
                                              m_SuffixesFlag);
                        if (suffix.startsWith(this_morph)){
                            remove_parse(this_parse.display_full());
                            qDebug() << 227 << "Removing parse:" << this_parse.display_full();
                        }
                    }
                }
            }
         }
         for (int affixno = 0; affixno < affixes_from_shorter_stem.count(); affixno++){
             this_affix = affixes_from_shorter_stem[affixno];
             if (this_affix.left(morph_length) == this_morph ){
                 doomed_affixes.append(this_affix);
                 morph_piece = this_affix.mid(morph_length);
                 if (morph_piece.length() == 0) { morph_piece = "NULL";}
                 if (! affixes_from_longer_stem.contains(morph_piece) ){
                     affixes_from_longer_stem.append(morph_piece);
                     qDebug() << 221 << "Added new suffix to morph" <<morph_piece;
                 }
                 if (m_SuffixesFlag){
                     foreach(word_stem_struct * this_word_stem_struct, p_sig_pair->shared_word_stems ){
                         foreach(QString suffix, affixes_from_shorter_stem){
                             CParse this_parse(this_word_stem_struct->shorter_stem,
                                                   suffix,
                                                   m_SuffixesFlag);
                             remove_parse(this_parse.display_full());
                             qDebug() << 248 << "Removing parse:" << this_parse.display_full();
                         }
                     }
                 }
                 qDebug() << 223 << this_affix << " deleted." ;
             }
          }
          affixes_from_shorter_stem.append(this_morph);


          CHypothesis * this_hypothesis = new CHypothesis( HT_affix_goes_to_signature,
                                                           this_morph,
                                                           pSig1_longer_stem->display(),
                                                           pSig2_shorter_stem->display(),
                                                           QString(), //ref_info.m_str_revised_sig,
                                                           QStringList( ), //ref_info.m_str_revised_sig.split('='),
                                                           p_sig_pair->get_number_of_words());
          m_Hypotheses->append(this_hypothesis);
          m_Hypothesis_map->insert (this_hypothesis->express_as_string(),  this_hypothesis);

    }


    step3_from_parses_to_stem_to_sig_maps("Splitting up complex morphemes");
    step4_create_signatures("Splitting up complex morphemes.");

}















