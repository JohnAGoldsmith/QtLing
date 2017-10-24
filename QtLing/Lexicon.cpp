#include <QPair>
#include <QList>
#include <QMap>
#include <QMapIterator>
#include <QMultiMap>
#include <QSet>
#include <QtDebug>
#include "Lexicon.h"

#include "SignatureCollection.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "WordCollection.h"
#include "Word.h"

CLexicon::CLexicon() : m_Words(new CWordCollection), m_Stems(new CStemCollection), m_Suffixes(new CSuffixCollection), m_Signatures(new CSignatureCollection)
{
    m_Parses                = new QList<QPair<QString,QString>>();
    m_Protostems            = QMap<QString, int>();
    m_ResidualSignatures    =  new CSignatureCollection();
    m_ResidualStems         = new CStemCollection();
    m_SingletonSignatures   = new CSignatureCollection();
    m_SingletonStems        = new CStemCollection();
}

QListIterator<sig_tree_edge*> * CLexicon::get_sig_tree_edge_list_iter()
{
    QListIterator<sig_tree_edge*> * iter = new QListIterator<sig_tree_edge*>(m_SigTreeEdgeList);
    return iter;
}

QMapIterator<QString, sig_tree_edge*> * CLexicon::get_sig_tree_edge_map_iter()
{
    QMapIterator<QString, sig_tree_edge*> * iter = new QMapIterator<QString, sig_tree_edge*>(m_SigTreeEdgeMap);
    return iter;
}

//linguistic methods

void CLexicon::Crab_1()
{
    FindProtostems();
    CreateStemAffixPairs();
    AssignSuffixesToStems();
    PurifyResidualSignatures();
    qDebug() << "finished making signatures.";
    find_good_signatures_inside_residual_signatures(true);
    compute_sig_tree_edges();
    compute_sig_tree_edge_map();
    qDebug() << "finished sorting multiparses";
}

/*!
 * This is the first of the three initial parts of finding signatures.
 * This makes a cut at every point in a word where the successor frequency
 * is greater than 1.
 */
void CLexicon::FindProtostems()
{   word_t          word, previous_word;
    QStringList *   Words =  GetWordCollection()->GetSortedStringArray();
    bool            StartFlag = true;
    bool            DifferenceFoundFlag = false;
    stem_t          stem;
    for (int wordno=0; wordno<Words->size(); wordno ++){
        word = Words->at(wordno);
        if (StartFlag){
            StartFlag = false;
            previous_word = word;
            continue;
        }
        DifferenceFoundFlag = false;
        int end = qMin(word.length(), previous_word.length());
        for (int i=0; i <end; i++){
            if (previous_word[i] != word[i]){
                stem = previous_word.left(i);
                DifferenceFoundFlag = true;
                if (!m_Protostems.contains(stem))                {
                    m_Protostems[stem] = 1;
                }
                break;
            }
        }
        if (DifferenceFoundFlag == true){
            previous_word = word;
            continue;
        }
        else {
            if (previous_word.length() < word.length()) {
                m_Protostems[previous_word] = 1;
            }
        }
        previous_word = word;
    }
    return;
}

/*!
 * This is the second of the three initial parts of finding signatures.
 * This creates stem/affix pairs, which are put in a long list of "Parses".
 */
void CLexicon::CreateStemAffixPairs()
{
    QString                     stem, suffix, word;
    int                         suffix_length;
    map_string_to_word_iter *   word_iter = m_Words->get_iterator();
    while (word_iter->hasNext())   {
        word = word_iter->next().value()->GetWord();
        for (int letterno = 1; letterno < word.length(); letterno++){
            stem = word.left(letterno);
            if (m_Protostems.contains(stem)){
                suffix_length = word.length() - letterno;
                suffix = word.right(suffix_length);
                m_Parses->append(QPair<QString,QString>(stem,suffix));
                if (m_Words->contains(stem)){
                    m_Parses->append(QPair<QString,QString>(stem,QString("NULL")));
                }
            }
        }
    }
}

/*!
 * This is the third of the three initial parts of finding signatures.
 * This creates signatures, which in turn creates stems and affixes.
 */
void   CLexicon::AssignSuffixesToStems()
{   const int MINIMUM_NUMBER_OF_STEMS = 2;

    QPair<QString,QString>      this_pair;
    CSignature*                 pSig;
    QString                     this_stem, this_suffix, this_signature_string, this_word;;
    CStem*                      pStem;
    map_sig_to_morph_set        temp_stems_to_affixes;
    map_sig_to_stem_list        temp_signatures_to_stems;
    for (int parseno = 0; parseno < m_Parses->size(); parseno++){
        this_pair = m_Parses->at(parseno);
        this_stem = this_pair.first;
        this_suffix = this_pair.second;
        if (! temp_stems_to_affixes.contains(this_stem)){
            morph_set * pSet = new QSet<QString>;
            temp_stems_to_affixes.insert(this_stem,pSet);
        }
        temp_stems_to_affixes.value(this_stem)->insert(this_suffix);
    }

    QMapIterator<QString, morph_set*>   stem_iter(temp_stems_to_affixes);
    while (stem_iter.hasNext())
    {    stem_iter.next();
         this_stem = stem_iter.key();
         QStringList temp_presignature;
         foreach (this_suffix, *temp_stems_to_affixes.value(this_stem)){
            temp_presignature.append(this_suffix);
         }
         temp_presignature.sort();
         sigstring_t this_signature_string = temp_presignature.join("=");
         if ( ! temp_signatures_to_stems.contains(this_signature_string)){
            stem_list * pStemSet = new stem_list;
            temp_signatures_to_stems[this_signature_string] = pStemSet;
         }
         temp_signatures_to_stems.value(this_signature_string)->append(this_stem);
    }
    //-->  create signatures, stems, affixes:  <--//
    QMapIterator<sigstring_t, stem_list*> iter_sigstring_to_stems ( temp_signatures_to_stems);
    while (iter_sigstring_to_stems.hasNext())
    {   iter_sigstring_to_stems.next();
        this_signature_string = iter_sigstring_to_stems.key();
        this_stem = iter_sigstring_to_stems.value()->first();
        suffix_set this_suffix_set = QSet<QString>::fromList( this_signature_string.split("="));
        if (iter_sigstring_to_stems.value()->size() >= MINIMUM_NUMBER_OF_STEMS)
        {
            QSetIterator<suffix_t> suffix_iter(this_suffix_set);
            while(suffix_iter.hasNext()){
                  this_suffix = suffix_iter.next();
                  CSuffix* pSuffix = m_Suffixes->find_or_add(this_suffix);
                  pSuffix->increment_count();
            }
            pSig = *m_Signatures<< this_signature_string;
            foreach (this_stem, *iter_sigstring_to_stems.value()){
                pStem = m_Stems->find_or_add(this_stem);
                pStem->add_signature (this_signature_string);
                pSig->add_stem_pointer(pStem);
                QStringList affixes = this_signature_string.split("=");
                foreach (suffix_t this_suffix,  affixes){
                    if (this_suffix == "NULL"){
                        this_word = this_stem;
                    } else{
                        this_word = this_stem + this_suffix;
                    }
                    CWord* pWord = m_Words->get_word(this_word);
                    pWord->add_stem_and_signature(pStem,pSig);
                    pWord->add_to_autobiography("First pass stem; " + this_signature_string);
                }
            }
        }else{
            this_signature_string =  iter_sigstring_to_stems.key();
            pSig =  *m_ResidualSignatures << this_signature_string;
            pStem = *m_ResidualStems << this_stem;
            pSig->add_stem_pointer(pStem);
        }
    }
}



/*!
 * We look inside the ResidualSignatures, and extract only the approved suffixes inside them.
 */
void   CLexicon::PurifyResidualSignatures()
{   stem_t                      this_stem;
    CStem*                      pStem;
    word_t                      this_word;
    sig_string                  this_signature_string;
    CSignature*                 pSig;
    int                         suffix_no;
    int                         count_of_new_stems = 0;
    int                         count_of_new_words = 0;
    QList<suffix_t>             true_suffix_list;
    suffix_set                  suffix_set2;

    dump_suffixes(&true_suffix_list);
    QSet<QString>               true_suffix_set = QSet<QString>::fromList(true_suffix_list);
                                m_Signatures->sort_signatures_by_affix_count();

    //---->   We iterate through the list of Residual Signatures <-------//

    QMapIterator<QString, CSignature*> * sig_iter =  m_ResidualSignatures->get_map_iterator();

    while (sig_iter->hasNext()){
        sig_iter->next();
        CSignature*         pResidualSig        = sig_iter->value();
        suffix_set          this_residual_suffix_set = QSet<suffix_t>::fromList( pResidualSig->get_key().split("="));
                            //--> The next line filters out all except true suffixes. <--//
                            this_residual_suffix_set.intersect(true_suffix_set);
                            if (this_residual_suffix_set.size() < 1) { continue; }
                            this_stem = pResidualSig->get_stems()->first()->get_key(); // there is only 1 stem in these signatures, by construction.
                            if (m_Words->contains(this_stem)){
                                this_residual_suffix_set.insert("NULL");
                            }
                            QStringList templist = this_residual_suffix_set.toList();
                            //--> Now we look for largest true signature inside this list of suffixes. <--//
                            for (int sig_no=0; sig_no < get_signatures()->get_count(); sig_no++){
                                suffix_set2.clear();
                                pSig = m_Signatures->get_at_sorted(sig_no);
                                pSig->dump_string_set_of_suffixes(suffix_set2);
                                QStringList stringlist1 = suffix_set2.toList();
                                QString string1 = stringlist1.join("=");
                                if (this_residual_suffix_set.contains(suffix_set2)){
                                    QList<QString> this_suffix_list = suffix_set2.toList();
                                    this_suffix_list.sort();

                                    sigstring_t signature_string_1 = this_suffix_list.join ("=");
                                    pSig =  *m_SingletonSignatures << signature_string_1;
                                    pStem = *m_SingletonStems << this_stem;
                                    pSig->add_stem_pointer(pStem);
                                    break;
                                }
                            }
    }
}


/*!
 *  This function looks at all the non-signatures that were rejected
 *  because they were associated with only one stem. For each one, it
 *  looks for the broadest signature that occurs inside it, and assigns
 *  its stem to that signature.
 */

struct{
    bool operator ()(CSignature* pSig_a, CSignature* pSig_b) const {
    // qDebug() << a->words.size() << b->words.size();
    //qDebug() << "244" << a->words.size() << a->words.first() << b->words.size() ;
    // return pSig_a->number_of_true_suffixes() > pSig_b->number_of_true_suffixes();
    }
}custom_compare_residual_sig;

void CLexicon::find_good_signatures_inside_residual_signatures(bool FindSuffixesFlag){


}




/*!
 * Replace parse pairs from current signature structure. This allows us to
 * delete the old signature structure, and replace them with the set of
 * parse-pairs that exactly describe the current signature structure.
 */

void CLexicon::replace_parse_pairs_from_current_signature_structure(bool FindSuffixesFlag) {
    m_Parses->clear();
    m_Parse_map.clear();
    QString                         sig_string;
    CSignature*                     pSig;
    CStem*                          pStem;
    QList<CStem*> *                 stem_list;
    map_sigstring_to_sigptr_iter  * sig_iter =  get_signatures()->get_map_iterator();

    while (sig_iter->hasNext()){
        pSig = sig_iter->next().value();
        stem_list =  pSig->get_stems();
    }
}





/*!
 * We can build a graph whose nodes are the signatures, where an edge connects
 * any pair of signatures if there exists a word that they both analyze.
 * A sig_tree_edge has two flavors: this function uses the flavor that
 * contains the two signatures, the word, and the string-difference between
 * the stems of the word at the two signatures.
 */
void CLexicon::compute_sig_tree_edges()
{
    CWord*                          pWord;
    map_string_to_word *            WordMap = m_Words->GetMap();
    map_string_to_word_iter         word_iter(*WordMap);
    sig_tree_edge *                 p_SigTreeEdge;

    while (word_iter.hasNext())   {
        pWord = word_iter.next().value();
        int number_of_signatures = pWord->GetSignatures()->size();
        if ( number_of_signatures > 2){
            for (int signo1=0; signo1 < number_of_signatures; signo1++){
                stem_sig_pair* pair1 =  pWord->GetSignatures()->value(signo1);
                CStem * stem1       = pair1->first;
                CSignature* sig1    = pair1->second;
                for (int signo2=signo1 + 1; signo2 < number_of_signatures; signo2++){
                    stem_sig_pair * pair2 = pWord->GetSignatures()->value(signo2);
                    CStem *  stem2       = pair2->first;
                    CSignature* sig2    = pair2->second;
                    if ( stem1->get_key().length() > stem2->get_key().length() ){
                        morph_t difference = stem1->get_key().mid(stem2->get_key().length());
                        p_SigTreeEdge = new sig_tree_edge (sig1,sig2,difference, pWord->get_key());
                    } else{
                        morph_t difference = stem2->get_key().mid(stem1->get_key().length());
                        p_SigTreeEdge =  new sig_tree_edge (sig2,sig1,difference, pWord->get_key());
                    }
                    m_SigTreeEdgeList.append(p_SigTreeEdge);
                }
            }
        }
    }
}

/*!
 * This function takes the SigTreeEdge list, and makes a smaller list composed of
 * SigTreeEdges which share the same signature pair and string-difference. This
 * flavor of sig_tree_edge contains a list of all the words that participate in
 * this particular sig_tree_edge.
 */
void CLexicon::compute_sig_tree_edge_map() {

morph_t         edge_label;
word_t          this_word;
sig_tree_edge * p_sig_tree_edge,
              * p_sig_tree_edge_2,
              * p_sig_tree_edge_3;
QMap<QString, sig_tree_edge*> * p_EdgeMap = & m_SigTreeEdgeMap;

QListIterator<sig_tree_edge*> sig_tree_edge_iter (m_SigTreeEdgeList);
while (sig_tree_edge_iter.hasNext())
{
    p_sig_tree_edge = sig_tree_edge_iter.next();
    edge_label = p_sig_tree_edge->label();
    this_word  = p_sig_tree_edge->word;
    if (p_EdgeMap->contains(edge_label)){
        p_sig_tree_edge_3 = p_EdgeMap->value(edge_label);
        p_sig_tree_edge_3->words.append(this_word);
    } else {
        sig_tree_edge * p_sig_tree_edge_2 = new sig_tree_edge(
            p_sig_tree_edge->sig_1,
            p_sig_tree_edge->sig_2,
            p_sig_tree_edge->morph
           );
        p_sig_tree_edge_2->words.append(this_word);
        p_EdgeMap->insert(p_sig_tree_edge_2->label(), p_sig_tree_edge_2);
    }
}
}


void CLexicon::dump_suffixes(QList<QString> * pList)
{
    return m_Suffixes->get_suffixes(pList);
}











