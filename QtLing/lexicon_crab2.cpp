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
#include "cparse.h"

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
   affix_set *                 this_ptr_to_affix_set;
   CStem*                      pStem;
   CStemCollection*            stems;
   m_SuffixesFlag ?
               stems = m_suffixal_stems:
               stems = m_prefixal_stems;
   map_sigstring_to_suffix_set   temp_stems_to_affix_set;
   map_sigstring_to_morph_set  & ref_stems_to_affix_set (temp_stems_to_affix_set);
   map_sigstring_to_stem_list    temp_signatures_to_stems;

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
   //--> THIS is where the continuations that are not affixes are eliminated -- well, they are not
   //    eliminated, but they are not copied into ref_stems_to_affix_set.

   create_temporary_map_from_stems_to_affix_sets( ref_stems_to_affix_set);//, ref_temp_signatures_to_stems);

   assign_suffixes_to_stems("re-signaturize to good affixes only");




if (false) {

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
        if (this_ptr_to_affix_set->size() < 2){continue;}
        QStringList temp_presignature;



        affix_set_iter affix_iter (*this_ptr_to_affix_set);
        while (affix_iter.hasNext()){
                temp_presignature.append ( affix_iter.next() );
        }

        temp_presignature.sort();
        sigstring_t this_signature_string = temp_presignature.join("=");
        if ( ! temp_signatures_to_stems.contains(this_signature_string)){
           stem_list * pStemSet = new stem_list;
           temp_signatures_to_stems[this_signature_string] = pStemSet;
        }
        temp_signatures_to_stems.value(this_signature_string)->append(this_stem_t);

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
           int stem_count = 0;
           while (stem_iter.hasNext()){
               this_stem_t = stem_iter.next();
               stem_count = 0;
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
                   pWord->add_parse_triple(this_stem_t, this_affix, this_signature_string);
                   QString message = this_signature_string;
                   if (affixes.size()> 50){message = "very long signature";}
                   pWord->add_to_autobiography("Resignaturize:=" + this_stem_t + "=" + message );
                   stem_count += pWord->get_word_count();
               }
               pStem->set_count(stem_count);
           }
       }
       else
       {       // insufficient number of stems ...
           this_signature_string =  iter_sigstring_to_stems.key();
           pSig =  *m_ParaSignatures << this_signature_string;
           pStem = *m_ResidualStems << this_stem_t;
           //qDebug() << this_stem_t << this_signature_string;
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
               pWord->add_to_autobiography("*singleton=" + this_stem_t + "=" + message );
           }
       }
   }
   m_Signatures->sort_each_signatures_stems_alphabetically();
} // end of if false...

}
/**
 * helper function for preceeding function.
 *
 */
void CLexicon::create_temporary_map_from_stems_to_affix_sets(map_sigstring_to_morph_set & ref_stems_to_affix_set//,
                                                             //map_sigstring_to_stem_list & ref_temp_signatures_to_stems
                                                             )
{
    CParse*                     this_parse;
   // QPair<QString,QString>      this_pair;
    QString                     this_stem_t, this_suffix_t;
    morph_set *                 pSet;
    CStemCollection *           stems;
    m_SuffixesFlag ?
                stems = m_suffixal_stems:
                stems = m_prefixal_stems;

    // iterate through parselist, and assign to stem and affix collections;
    for (int parseno = 0; parseno < m_Parses->size(); parseno++){
        this_parse = m_Parses->at(parseno);
        m_ProgressBar->setValue(parseno);
        if (m_SuffixesFlag){
            this_stem_t = this_parse->get_string1();
            this_suffix_t = this_parse->get_string2();
            if (! m_Suffixes->contains(this_suffix_t)){
                continue;
            }
        } else{
            this_stem_t = this_parse->get_string2();
            this_suffix_t = this_parse->get_string1();
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
    //sig_string                  this_signature_string;
    CStem*                      pStem;
    CWord*                      pWord;
    CSignature*                 p_proven_sig;
    affix_list                  affixes_of_residual_sig;
    CSuffix_ptr_list            this_residual_sig_suffix_pointer_list;
    CSignatureCollection*       signatures;
    suffix_t                    Null_string ("NULL");
    CStemCollection*            stems;
    m_SuffixesFlag ?
                stems = m_suffixal_stems:
                stems = m_prefixal_stems;

    affix_list                  best_affix_list;

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

    signatures->find_minimal_cover(); // the signature collection finds a "cover", which is a set of signatures which are a superset of all the other signatures.

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
        //`qDebug() << 308 << this_stem;
        if (this_stem=="toto"){
               int z = 1;
        }
        int stem_length = this_stem.length();

        for (int wordno= this_protostem->get_start_word(); wordno <= this_protostem->get_end_word(); wordno++){
            QString this_word, affix;
            if (m_SuffixesFlag){
                this_word = m_Words->get_word_string(wordno);
                affix = this_word.mid( stem_length );
            } else{
                this_word = m_Words->get_reverse_sort_list()->at(wordno);
                affix = this_word.left(this_word.length()- stem_length);
            }
            affixes_of_residual_sig.append( affix );
        }
        if (m_Words->contains(this_stem)) {
                affixes_of_residual_sig.append("NULL");
        }



        // the next few lines is the beginning of the rewrite of the messy stuff that follows it. March 24 2018.
        // 1. Find best subsignature.
        best_affix_list.clear();
        affix_list affix_intersection;
        for (int i = 0; i <signatures->get_minimal_cover()->size(); i++ ){
            CSignature* pSig = signatures->get_minimal_cover()->at(i);
            pSig->intersection_with(affixes_of_residual_sig, affix_intersection);
            if ( affix_intersection.length() > 1 &&
                 affix_intersection.size() > best_affix_list.size()){
                 best_affix_list = affix_intersection;
            }
        }
        if (best_affix_list.size() == 0) {
            //qDebug() << this_stem << affixes_of_residual_sig << "\n";
            continue;
        }

        // 2. add the signature string to the signature collection:
        CSignature * pSig;
        QString best_affix_list_string = best_affix_list.join("=");

        pSig = *signatures << best_affix_list_string;
        if (!m_SuffixesFlag){ pSig->set_suffix_flag(false);}

        QListIterator<QString> affix_iter_2(best_affix_list);
        while(affix_iter_2.hasNext()){
            this_affix = affix_iter_2.next();
            link_signature_and_affix(pSig,this_affix);
        }
        link_signature_and_stem(this_stem, pSig, best_affix_list_string);

        // . This shouldn't happen: there are no affixes.
        if (best_affix_list.length() == 0){
            qDebug() << 359 << "affix list with no entries; this should not happen."<<this_stem << best_affix_list;
            continue;
        }
// we shouldn't need 4 and 5:
        if (false){
        // 4. Deal with this stem. Create a cstem if it did not already exist. Link the signature and the stem.
        pStem = stems->find_or_add(this_stem);
        pStem->add_signature(pSig);
        pSig->add_stem_pointer(pStem);
        //--> add to autobiographies <--//

        // 5. find the caffixes for each affix. Add parses to each cword.
        for (int affixno = 0; affixno < best_affix_list.length(); affixno++){
            this_affix = best_affix_list[affixno];
            if (this_affix == "NULL"){
                this_word= this_stem;
            }else{
                m_SuffixesFlag?
                    this_word = this_stem + this_affix:
                    this_word = this_affix + this_stem;
            }
            pWord = m_Words->find_or_fail(this_word);
            if (pWord){
                pWord->add_parse_triple(this_stem, this_affix, best_affix_list.join("="));
                pWord->add_to_autobiography("Good sig found within bad: ="  + this_stem  + "=" +  best_affix_list.join("="));
            }
        }

        // 6. Make sure each affix has a c_affix.
        QListIterator<affix_t> affix_iter(best_affix_list);
        while(affix_iter.hasNext()){
              this_affix = affix_iter.next();
              if (m_SuffixesFlag){
                  CSuffix* pSuffix = m_Suffixes->find_or_add(this_affix);
                  pSuffix->increment_count();
                  pSig->add_affix_ptr(pSuffix);
              } else {
                  CPrefix* pPrefix = m_Prefixes->find_or_add(this_affix);
                  pPrefix->increment_count();
                  pSig  ->add_affix_ptr(pPrefix);
              }
        }
        }






    }

     signatures->sort_each_signatures_stems_alphabetically();
}

