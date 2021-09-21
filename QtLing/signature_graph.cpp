#include "Lexicon.h"
#include "WordCollection.h"
#include "Word.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "Lexicon.h"
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
    CSignature *                    shorter_stem_sig_ptr, * longer_stem_sig_ptr;
    CWord*                          pWord;
    morph_t                         difference;
    stem_t                          this_stem, the_other_stem;
    int                             analysis_number = 0, length_of_difference;
    word_t          this_word;
    sig_pair        * p_sig_pair;
    QString         shorter_stem, longer_stem, shorter_stem_sig_string, longer_stem_sig_string, label;
    Parse_triple *  shorter_stem_triple, * longer_stem_triple;



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

            for (int j = i+1; j <pWord->get_parse_triple_list()->size(); j++ )
            {   Parse_triple* the_other_triple = pWord->get_parse_triple_list()->at(j);
                the_other_stem = the_other_triple->m_stem;                
                if (this_stem.length() > the_other_stem.length()){
                    shorter_stem = the_other_stem;
                    longer_stem = this_stem;
                    shorter_stem_triple = the_other_triple;
                    longer_stem_triple = this_triple;
                } else{
                    shorter_stem = this_stem;
                    longer_stem = the_other_stem;
                    shorter_stem_triple = this_triple;
                    longer_stem_triple = the_other_triple;
                }
                length_of_difference = longer_stem.length() - shorter_stem.length();
                if (m_SuffixesFlag)
                {
                    shorter_stem_sig_ptr = m_Signatures->find_or_fail(shorter_stem_triple->m_sig_string);
                    longer_stem_sig_ptr = m_Signatures->find_or_fail(longer_stem_triple->m_sig_string);
                    difference = longer_stem.right(length_of_difference);
                }else{
                    shorter_stem_sig_ptr = m_PrefixSignatures->find_or_fail(shorter_stem_triple->m_sig_string);
                    longer_stem_sig_ptr = m_PrefixSignatures->find_or_fail(longer_stem_triple->m_sig_string);
                    difference = longer_stem.left(length_of_difference);
                }
                shorter_stem_sig_string = shorter_stem_sig_ptr->display();
                longer_stem_sig_string = longer_stem_sig_ptr->display();
                if ( shorter_stem_sig_ptr == longer_stem_sig_ptr){continue;}
                label = difference + "/" + shorter_stem_sig_string + "/" + longer_stem_sig_string;

                if (m_SigPairMap.contains(label)){
                    p_sig_pair = m_SigPairMap[label];
                    if (p_sig_pair->contains_stem(shorter_stem)){
                        //qDebug() << "skipping "<< shorter_stem;
                        continue;
                    }
                    p_sig_pair->add_stem(shorter_stem);
                    //qDebug() << "  add new stem "<< shorter_stem << label;
                }else{
                    p_sig_pair = new sig_pair(difference, shorter_stem_sig_string, longer_stem_sig_string);
                    p_sig_pair->add_stem(shorter_stem);
                    m_SigPairMap.insert(label, p_sig_pair);
                    //qDebug() << "  add pair and stem"<< shorter_stem << label << "count "<<m_SigPairMap.count();
                }



                QString message1 = "sig graph edge #" + QString::number(analysis_number) + "=signature 1:="
                        + shorter_stem + "/" +  shorter_stem_sig_string + "/" + "difference" + "="  + difference;
                add_to_word_autobiographies(pWord->get_key(), message1);
                QString message2 = "=signature 2:   =" + longer_stem + "=" + longer_stem_sig_string;
                add_to_word_autobiographies(pWord->get_key(),message2);
                //qDebug() << "\n" << this_word<< i << j << analysis_number;
                //qDebug() << label;
                //qDebug() << message1;
                //qDebug() << message2;
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
                qDebug() << 201 << "      Added new suffix to morph" << remnant;
            }
        }
    }
}
void CLexicon::step8c_from_sig_pairs_to_parses(){
    sig_pair_iter sig_pair_iter (m_SigPairMap);
    QString     this_affix, doomed_affix, remnant, difference;
    QString shorter_stem, longer_stem;
    QString shorter_stem_sig_string, longer_stem_sig_string;
    QStringList affected_signatures;
    sig_pair*   p_sig_pair;
    int MINIMUM_NUMBER_OF_WORDS = M_MINIMUM_HYPOTHESIS_WORD_COUNT; // usually 2
    int difference_length;
    while (sig_pair_iter.hasNext())
    {
         p_sig_pair = sig_pair_iter.next().value();

         QString difference = p_sig_pair->get_morph();
         difference_length = difference.length();
         if (difference_length < 2){ continue;}
         /*
         if (p_sig_pair->get_sig_1()->get_stem_entropy() < m_entropy_threshold_for_stems) {continue;}
         if (p_sig_pair->get_sig_2()->get_stem_entropy() < m_entropy_threshold_for_stems) {continue;}
         */
         if (p_sig_pair->get_number_of_words() < MINIMUM_NUMBER_OF_WORDS ){ continue;}

         QStringList affixes_from_shorter_stem = p_sig_pair->get_sig1_string().split("=");
         QStringList affixes_from_longer_stem  = p_sig_pair->get_sig2_string().split("=");

         if (m_SuffixesFlag){
             // remove parses of longer-stem + affixes in long-stem signature.
             remove_parses_1 (this, p_sig_pair->get_my_stems(), affixes_from_longer_stem, difference);

             // add suffixes to long-stem signature if necessary
             add_suffixes_to_signature_stringlist(difference, affixes_from_shorter_stem, affixes_from_longer_stem);

             // remove parses made from short-stem plus suffixes that start with difference
             remove_parses_2(this, p_sig_pair->get_my_stems(), affixes_from_shorter_stem, difference);

             // add a parse of difference (=stem) + each of the suffixes of the long-stem signature.
             foreach (QString suffix, affixes_from_longer_stem){
                 CParse * pParse = new CParse(difference, suffix, m_SuffixesFlag);
                 add_parse(pParse);
             }
             // add a parse of shorter-stem plus difference
             foreach (QString stem, p_sig_pair->get_my_stems()){
                 CParse * pParse = new CParse(stem, difference, m_SuffixesFlag);
                 add_parse(pParse);
             }
         }
         affixes_from_longer_stem.sort();
         longer_stem_sig_string = affixes_from_longer_stem.join("=");

          CHypothesis * this_hypothesis = new CHypothesis( HT_affix_goes_to_signature,
                                                           difference,
                                                           longer_stem_sig_string,
                                                           p_sig_pair->get_sig1_string(),
                                                           p_sig_pair->get_number_of_words());
          m_Hypotheses->append(this_hypothesis);
          m_Hypothesis_map->insert (this_hypothesis->express_as_string(),  this_hypothesis);

    }


    step3_from_parses_to_stem_to_sig_maps("Splitting up complex morphemes");
    step4_create_signatures("Splitting up complex morphemes.");
}















