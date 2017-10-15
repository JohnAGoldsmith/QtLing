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

typedef QSet<QString>             morph_set;
typedef QSet<QString>             stem_set;
typedef QMap<QString, morph_set*> map_sig_to_morph_set;
typedef QMap<QString, CWord*>     map_string_to_word;
typedef QMap<QString, CSignature*> map_string_to_sig;
typedef QMapIterator<QString, CWord*> map_string_to_word_iter;
typedef QList<QString>            word_list;

//typedef  QPair<QString, CSignature*>                    sig_and_pointer;
typedef  QPair<CSignature*, CSignature*>                pair_of_sigs;
typedef  QPair<QString, pair_of_sigs*>                  multiparse_edge_label;
typedef  QPair<word_list,multiparse_edge_label*>        multiparse;

CLexicon::CLexicon() : m_Words(new CWordCollection), m_Stems(new CStemCollection), m_Suffixes(new CSuffixCollection), m_Signatures(new CSignatureCollection)
{
    m_Parses = new QList<QPair<QString,QString>>();
    m_Protostems = QMap<QString, int>();
    m_RawSignatures =  new CSignatureCollection();
}

//linguistic methods

void CLexicon::Crab_1()
{
    FindProtostems();
    CreateStemAffixPairs();
    AssignSuffixesToStems();
    qDebug() << "finished making signatures.";
    ComputeMultiparses();
    SortMultiparses(1);
    qDebug() << "finished sorting multiparses";
}

void CLexicon::FindProtostems()
{  QString word, previous_word;
    QStringList * Words =  GetWordCollection()->GetSortedStringArray();
    bool StartFlag = true;
    bool DifferenceFoundFlag = false;
    QString stem;
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
                if (!m_Protostems.contains(stem))
                {
                    m_Protostems[stem] = 1;
                }
                break;
            }
        }
        if (DifferenceFoundFlag == true)
        {
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

void CLexicon::CreateStemAffixPairs()
{
    QString stem, suffix, word;
    int suffix_length;
    map_string_to_word_iter * word_iter = m_Words->get_iterator();
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
void   CLexicon::AssignSuffixesToStems()
{
    const int MINIMUM_NUMBER_OF_STEMS = 2;
    QPair<QString,QString> this_pair;
    CSignature* pSig;
    QString stem, affix, signature_string, word;;
    CStem* pStem;
    map_sig_to_morph_set  temp_stems_to_affixes;
    map_sig_to_morph_set  temp_signatures_to_stems;

    for (int parseno = 0; parseno < m_Parses->size(); parseno++){
        this_pair = m_Parses->at(parseno);
        stem = this_pair.first;
        affix = this_pair.second;
        if (! temp_stems_to_affixes.contains(stem)){
            morph_set * pSet = new QSet<QString>;
            temp_stems_to_affixes.insert(stem,pSet);
        }
        temp_stems_to_affixes.value(stem)->insert(affix);
    }
    // Do we need to delete the QSets, or is their memory taken care of? Probably need to delete them...and below.

    QMapIterator<QString, morph_set*>   stem_iter(temp_stems_to_affixes);
    while (stem_iter.hasNext())
    {
         stem = stem_iter.next().key();
         QStringList temp_presignature;
         foreach (affix, *temp_stems_to_affixes.value(stem)){
            temp_presignature.append(affix);           
         }
         temp_presignature.sort();
         QString sig_string = temp_presignature.join("=");
         if ( ! temp_signatures_to_stems.contains(sig_string)){
            morph_set * pStemSet = new QSet<QString>;
            temp_signatures_to_stems.insert(sig_string,pStemSet);
         }
         temp_signatures_to_stems.value(sig_string)->insert(stem);
    }

    // create signatures, stems, affixes:

    QMapIterator<QString, morph_set*> iter_sigstring_to_stems ( temp_signatures_to_stems);
    while (iter_sigstring_to_stems.hasNext())
    {
        signature_string = iter_sigstring_to_stems.next().key();
        QStringList affixes_set = signature_string.split("=");
        if (iter_sigstring_to_stems.value()->size() >= MINIMUM_NUMBER_OF_STEMS)
        {
            QListIterator<QString> affix_iter(affixes_set);
            while(affix_iter.hasNext()){
                  affix = affix_iter.next();
                  m_Suffixes->find_or_add(affix);                   
            }
            pSig = *m_Signatures<< signature_string;
            foreach (stem, *iter_sigstring_to_stems.value()){
                pStem = m_Stems->find_or_add(stem);
                pStem->add_signature (signature_string);
                pSig->add_stem_pointer(pStem);
                QStringList affixes = signature_string.split("=");

                foreach (QString affix,  affixes){
                    if (affix == "NULL"){
                        word = stem;
                    } else{
                        word = stem + affix;
                    }
                    CWord* pWord = m_Words->get_word(word);
                    pWord->add_stem_and_signature(pStem,pSig);
                }
            }
        }else{
            signature_string =  iter_sigstring_to_stems.key();
            *m_RawSignatures << signature_string;
            // We have not done anything with the individual affixes or stems yet.
        }
    }
}

void CLexicon::find_good_signatures_inside_raw_signature(bool FindSuffixesFlag){



}





// Replace parse pairs from current signature structure. This allows us to delete the old signature structure, and replace them with the set of parse-pairs that exactly descreibe the current signature structure
void CLexicon::replace_parse_pairs_from_current_signature_structure(bool FindSuffixesFlag) {
    m_Parses->clear();
    m_Parse_map.clear();
    QString         sig_string;
    CSignature*     pSig;
    CStem*          pStem;
    QList<CStem*> * stem_list;
    QMapIterator<QString,CSignature*>  * sig_iter =  get_signatures()->get_map_iterator();
    while (sig_iter->hasNext()){
        pSig = sig_iter->next().value();
        stem_list =  pSig->get_stems();
    }
}

//
//          Compute Multiparses

typedef  QPair<CStem*,CSignature*>  stem_sig_pair;
typedef  QPair<stem_sig_pair*,  stem_sig_pair*> pair_of_stem_sig_pairs;
typedef  QPair<QString, pair_of_stem_sig_pairs*> five_tuple_sig_diffs;
void CLexicon::ComputeMultiparses()
{
    CWord* pWord;
    map_string_to_word * WordMap = m_Words->GetMap();
    QMapIterator<QString,CWord*> word_iter(*WordMap);
    five_tuple_sig_diffs *  five_tuple;
    int tempcount = 0;

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
                        QString difference = stem1->get_key().mid(stem2->get_key().length());
                        pair_of_stem_sig_pairs * super_pair = new pair_of_stem_sig_pairs (pair1, pair2);
                        five_tuple = new five_tuple_sig_diffs (difference, super_pair);
                    } else{
                        QString difference = stem2->get_key().mid(stem1->get_key().length());
                        pair_of_stem_sig_pairs * super_pair = new pair_of_stem_sig_pairs (pair2, pair1);
                        five_tuple = new five_tuple_sig_diffs (difference, super_pair);
                    }
                    m_Multiparses.append( five_tuple);
                    tempcount ++;
                }
            }
        }
    }
 //qDebug() << "multiparse count"<<tempcount;
}

void CLexicon::SortMultiparses(int styleno){

    const five_tuple_sig_diffs * this_multiparse;
    pair_of_stem_sig_pairs * this_pair_of_stem_sig_pairs;
    stem_sig_pair * stem_sig_pair_1, * stem_sig_pair_2;
    int mp_no;
    QString difference;
    QString stem1, stem2;
    CSignature* sig1, *sig2;
    QString sig_string1, sig_string2;
    if (styleno == 1) {

    for (mp_no = 0; mp_no < m_Multiparses.size(); mp_no++){
        this_multiparse =  m_Multiparses.at(mp_no);
        difference = this_multiparse->first;
        this_pair_of_stem_sig_pairs = this_multiparse->second;
        stem_sig_pair_1 = this_pair_of_stem_sig_pairs->first;
        stem_sig_pair_2 = this_pair_of_stem_sig_pairs->second;
        stem1 = stem_sig_pair_1->first->get_key();
        sig1 = stem_sig_pair_1->second;
        sig_string1 = sig1->get_key();
        stem2 = stem_sig_pair_2->first->get_key();
        sig2 = stem_sig_pair_2->second;
        sig_string2 = sig2->get_key();
        pair_of_sigs * this_pair_of_sigs = new pair_of_sigs(sig1,sig2);
        multiparse_edge_label * this_multiparse_edge_label = new multiparse_edge_label (difference,this_pair_of_sigs);
        QString abbreviation = difference + QString("/") + sig1->display() + QString( "/")  + sig2->display();
        if (! m_Multiparse_edges.contains(abbreviation)) {
            m_Multiparse_edges[abbreviation] = this_multiparse_edge_label;
        }
      }
    }
}
























