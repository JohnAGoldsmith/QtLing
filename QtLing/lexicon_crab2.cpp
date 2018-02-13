#include <QPair>
#include <QList>
#include <QMap>
#include <QMapIterator>
#include <QMultiMap>
#include <QSet>
#include <QtDebug>
#include <QProgressBar>
#include <QApplication>
#include <algorithm>
#include <QChar>
#include "Lexicon.h"

#include "SignatureCollection.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "WordCollection.h"
#include "Word.h"

extern bool contains(QList<QString> * list2, QList<QString> * list1);


/**
 * @brief CLexicon::Crab_2
 * Crab_2 is a bit slower than Crab_1.
 */
void CLexicon::Crab_2()
{
    ReSignaturizeWithKnownAffixes();

    FindGoodSignaturesInsideParaSignatures();

     m_SuffixesFlag ?
        m_Signatures->calculate_stem_entropy():
        m_PrefixSignatures->calculate_stem_entropy();

    compute_sig_graph_edges();
    compute_sig_graph_edge_map();
    generate_hypotheses();


    qDebug() << "finished crab 2.";

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
* This is lke AssignSuffixesToStems, but crucially: with Suffixes known ahead of time.
* This creates signatures and stems: signatures that only have approved affixes.
* First function of Crab_2.
* I need to refactorize this function.
*/
void CLexicon::ReSignaturizeWithKnownAffixes()

{   const int MINIMUM_NUMBER_OF_STEMS = 2;
   CWord *                     pWord;
   CSignature*                 pSig;
   QString                     this_stem_t, this_suffix_t, this_prefix, this_affix, this_signature_string, this_word;
   stem_list *                 p_this_stem_list;
//   suffix_set *                this_ptr_to_suffix_set;
   affix_set *                 this_ptr_to_affix_set;
   CStem*                      pStem;
   CStemCollection*            stems;
   m_SuffixesFlag ?
               stems = m_suffixal_stems:
               stems = m_prefixal_stems;
   map_sigstring_to_suffix_set   temp_stems_to_affix_set;
   map_sigstring_to_morph_set  & ref_stems_to_affix_set (temp_stems_to_affix_set);
   map_sigstring_to_stem_list    temp_signatures_to_stems;
   //map_sigstring_to_stem_list  & ref_temp_signatures_to_stems(temp_signatures_to_stems);

   m_StatusBar->showMessage("Resignaturize with known affixes");
   m_ProgressBar->reset();
   m_ProgressBar->setMinimum(0);
   m_ProgressBar->setMaximum(m_Parses->size());

   map_string_to_word_ptr_iter word_iter (*m_Words->get_map());
   while(word_iter.hasNext()){
       pWord = word_iter.next().value();
       pWord->clear_signatures();
       pWord->clear_parse_triple_map();
   }
   //--> We establish a temporary map from stems to sets of affixes as we iterate through parses. <--//

   create_temporary_map_from_stems_to_affix_sets( ref_stems_to_affix_set);//, ref_temp_signatures_to_stems);

   //--> We iterate through these stems and for each stem, create QStringLists of their affixes. <--//
   //--> then we create a "pre-signature" in a map that points to lists of stems. <--//
   QMapIterator<QString, morph_set*>   stem_iter(temp_stems_to_affix_set);                       // part 1
   m_ProgressBar->reset();
   m_ProgressBar->setMinimum(0);
   m_ProgressBar->setMaximum(temp_stems_to_affix_set.size());
   int stem_count = 0;
   while (stem_iter.hasNext())                                                                  // make a presignature for each stem.
   {    stem_iter.next();
        m_ProgressBar->setValue(stem_count++);
        this_stem_t            = stem_iter.key();
        this_ptr_to_affix_set  = stem_iter.value();
        if (this_stem_t == "thro"){
            qDebug() << 607 << "thro" << this_ptr_to_affix_set ;
        }
        if (this_ptr_to_affix_set->size() < 2){continue;}
        QStringList temp_presignature;


        affix_set_iter affix_iter (*this_ptr_to_affix_set);
        while (affix_iter.hasNext()){
                temp_presignature.append ( affix_iter.next() );
        }

        temp_presignature.sort();
        sigstring_t this_signature_string = temp_presignature.join("=");
        if (this_stem_t == "thro"){
            qDebug() << this_signature_string << 621;
        }
        if ( ! temp_signatures_to_stems.contains(this_signature_string)){
           stem_list * pStemSet = new stem_list;
           temp_signatures_to_stems[this_signature_string] = pStemSet;
        }
        temp_signatures_to_stems.value(this_signature_string)->append(this_stem_t);
//        if (this_stem_t == "school"){
//            qDebug() << this_signature_string <<542;
//        }
   }

   //-->  create signatures, stems, affixes:  <--//
   if (m_SuffixesFlag){
       m_suffixal_stems->clear();
       m_Signatures->clear();
   }else {
       m_prefixal_stems->clear();
       m_PrefixSignatures->clear();
   }
   QMapIterator<sigstring_t, stem_list*> iter_sigstring_to_stems ( temp_signatures_to_stems);
   while (iter_sigstring_to_stems.hasNext())
   {   iter_sigstring_to_stems.next();
       this_signature_string    = iter_sigstring_to_stems.key();
       p_this_stem_list         = iter_sigstring_to_stems.value();
       this_stem_t              = p_this_stem_list->first();
       affix_set this_affix_set = QSet<QString>::fromList( this_signature_string.split("="));
       if (p_this_stem_list->size() >= MINIMUM_NUMBER_OF_STEMS)
       {   m_SuffixesFlag ?
               pSig = *m_Signatures       << this_signature_string :
               pSig = *m_PrefixSignatures << this_signature_string;
           pSig->add_memo("Pass 2");
           QSetIterator<suffix_t> affix_iter(this_affix_set);
           while(affix_iter.hasNext()){
                 this_affix = affix_iter.next();
                 if (m_SuffixesFlag){
                     CSuffix* pSuffix = m_Suffixes->find_or_add(this_affix);
                     pSuffix->increment_count();
                     pSig->add_affix_ptr(pSuffix);
                 }else{
                     CPrefix* pPrefix = m_Prefixes->find_or_add(this_affix);
                     pPrefix->increment_count();
                     pSig->add_affix_ptr(pPrefix);
                 }
           }

           // --> We go through this sig's stems and reconstitute its words. <--//
           stem_list_iterator stem_iter(*p_this_stem_list);
           while (stem_iter.hasNext()){
               this_stem_t = stem_iter.next();

               pStem = stems->find_or_add(this_stem_t);
               pStem->add_signature (pSig);
               pSig->add_stem_pointer(pStem);
               pStem->add_memo ("Pass1= ");
               QStringList affixes = this_signature_string.split("=");
               foreach (affix_t this_affix,  affixes){
                   if (this_affix == "NULL"){
                       this_word = this_stem_t;
                   } else{
                       m_SuffixesFlag ?
                           this_word = this_stem_t + this_affix :
                           this_word = this_affix + this_stem_t ;
                   }
                   CWord* pWord = m_Words->get_word(this_word);
                   //pWord->add_stem_and_signature(pStem,pSig);
                   pWord->add_parse_triple(this_stem_t, this_affix, this_signature_string);
                   //qDebug() << pSig->get_key() << 594;
                   QString message = this_signature_string;
                   if (affixes.size()> 50){message = "very long signature";}
                   pWord->add_to_autobiography("Pass2= " + this_stem_t + "=" + message );
               }
           }
       }else{       // insufficient number of stems ...
           this_signature_string =  iter_sigstring_to_stems.key();
           pSig =  *m_ParaSignatures << this_signature_string;
           pStem = *m_ResidualStems << this_stem_t;
           if (this_stem_t == "thro"){
               qDebug() << this_signature_string << 693;
           }
           pSig->add_stem_pointer(pStem);
           foreach (this_affix, this_affix_set){
               if (this_affix == "NULL"){
                   this_word = this_stem_t;
               } else{
                   m_SuffixesFlag ?
                      this_word = this_stem_t + this_affix :
                      this_word = this_affix + this_stem_t;
               }
               pWord = m_Words->find_or_fail(this_word);
               QString message = this_signature_string;
               if (this_affix_set.size()> 50){message = "very long signature";}
               pWord->add_to_autobiography("*" + this_word + "=" + this_stem_t + "=" + message );
           }
       }
   }
   m_Signatures->sort_each_signatures_stems_alphabetically();
}

/**
 * helper function for preceeding function.
 *
 */
void CLexicon::create_temporary_map_from_stems_to_affix_sets(map_sigstring_to_morph_set & ref_stems_to_affix_set//,
                                                             //map_sigstring_to_stem_list & ref_temp_signatures_to_stems
                                                             )
{
    QPair<QString,QString>      this_pair;
    QString                     this_stem_t, this_suffix_t;
    morph_set *                 pSet;
    CStemCollection *           stems;
    m_SuffixesFlag ?
                stems = m_suffixal_stems:
                stems = m_prefixal_stems;

    // iterate through parselist, and assign to stem cand affix collections;
    for (int parseno = 0; parseno < m_Parses->size(); parseno++){
        this_pair = m_Parses->at(parseno);
        m_ProgressBar->setValue(parseno);
        if (m_SuffixesFlag){
            this_stem_t = this_pair.first;
            this_suffix_t = this_pair.second;
            if (! m_Suffixes->contains(this_suffix_t)){
                continue;
            }
        } else{
            this_stem_t = this_pair.second;
            this_suffix_t = this_pair.first;
            if (! m_Prefixes->contains(this_suffix_t)){
                continue;
            }
        }

        if (! ref_stems_to_affix_set.contains(this_stem_t)){
            if (m_SuffixesFlag){
                pSet = new suffix_set();
            } else{
                pSet = new prefix_set();
            }
            ref_stems_to_affix_set.insert(this_stem_t,pSet);
        }
        ref_stems_to_affix_set.value(this_stem_t)->insert(this_suffix_t);
        }
}

/*!
 *  This function looks at all the non-signatures that were rejected
 *  because they were associated with only one stem. For each one, it
 *  looks for the broadest signature that occurs inside it, and assigns
 *  its stem to that signature.
 *
 * 2nd function of Crab 2.
 */


void   CLexicon::FindGoodSignaturesInsideParaSignatures()
{   stem_t                      this_stem;
    word_t                      this_word;
    affix_t                     this_affix;
    sig_string                  this_signature_string;
    CStem*                      pStem;
    CWord*                      pWord;
    CSignature*                 p_proven_sig;
    //CSuffix*                    pSuffix1;
    //int                         suffix_no;
    //int                         count_of_new_stems = 0;
    //int                         count_of_new_words = 0;
    //int                         signature_count (0);
    affix_list                  affixes_of_residual_sig;
    //CSuffix_ptr_list  *         list_of_CSuffixes_of_proven_sig;
    CSuffix_ptr_list            this_residual_sig_suffix_pointer_list;
    //bool                        success_flag;
    CSignatureCollection*       signatures;
    suffix_t                    Null_string ("NULL");
    //CSuffix*                    pNullSuffix = *m_Suffixes << Null_string;
    CStemCollection*            stems;
    m_SuffixesFlag ?
                stems = m_suffixal_stems:
                stems = m_prefixal_stems;

                                m_ProgressBar->reset();
                                m_ProgressBar->setMinimum(0);
                                m_ProgressBar->setMaximum(m_ParaSignatures->get_count());
                                m_StatusBar->showMessage("Find good signatures inside bad.");

    //---->   We iterate through the list of Residual Signatures <-------//

    if (m_SuffixesFlag) {
         signatures = m_Signatures;
    } else{
         signatures = m_PrefixSignatures;
    }
    signatures->sort(SIG_BY_AFFIX_COUNT);
    map_sigstring_to_sig_ptr_iter   sig_iter(*  m_ParaSignatures->get_map());

    QMap<QString, protostem*> * these_protostems_2;
    m_SuffixesFlag?
                these_protostems_2 = & m_suffix_protostems_2:
                these_protostems_2 = & m_prefix_protostems_2;
    int protostem_count = 0;
    m_ProgressBar->setMaximum(these_protostems_2->count());
    foreach (auto this_protostem, * these_protostems_2)
    {
        affixes_of_residual_sig.clear();
        m_ProgressBar->setValue(protostem_count++);
        stem_t this_stem = this_protostem->get_stem();
        int stem_length = this_stem.length();

        for (int wordno= this_protostem->get_start_word(); wordno <= this_protostem->get_end_word(); wordno++){
            QString this_word = m_Words->get_word_string(wordno);
            QString ending = this_word.mid( stem_length );
            affixes_of_residual_sig.append( ending );
        }
        if (m_Words->contains(this_stem)) {
                affixes_of_residual_sig.append("NULL");
        }
        //--> Now we look for largest true signature inside this list of suffixes. <--//
        //--> Inner loop, over all good signatures. <--//
        for (int sig_no=0; sig_no < signatures->get_count(); sig_no++){
            p_proven_sig = signatures->get_at_sorted(sig_no);
            QString p_proven_sigstring  = p_proven_sig->get_key();
            QList<QString> proven_sig_list = p_proven_sigstring.split("=");
            if ( contains(&affixes_of_residual_sig, &proven_sig_list) ){
                // We have found the longest signature contained in this_residual_suffix_set
                pStem = stems->find_or_add(this_stem);
                pStem->add_signature(p_proven_sig);
                p_proven_sig->add_stem_pointer(pStem);

                //--> add to autobiographies <--//

                for (int affixno = 0; affixno < proven_sig_list.length(); affixno++){
                    this_affix = proven_sig_list[affixno];
                    if (this_affix == "NULL"){
                        this_word= this_stem;
                    }else{
                        m_SuffixesFlag?
                            this_word = this_stem + this_affix:
                            this_word = this_affix + this_stem;
                    }
                    pWord = m_Words->find_or_fail(this_word);
                    if (pWord){
                        //pWord->add_stem_and_signature(pStem, p_proven_sig);
                        pWord->add_parse_triple(this_stem, this_affix, p_proven_sigstring);
                        pWord->add_to_autobiography("from within parasigs="  + this_stem  + "=" +  p_proven_sigstring);
                    }
                }
                break;
            }
        } // loop over proven signatures;
    }

     signatures->sort_each_signatures_stems_alphabetically();
}

