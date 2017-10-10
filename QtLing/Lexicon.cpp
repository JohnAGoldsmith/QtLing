#include <QPair>
#include <QList>
#include <QMap>
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
    m_Parses = new QList<QPair<QString,QString>>();
    m_Protostems = QMap<QString, int>();
}

//linguistic methods

void CLexicon::Crab_1()
{
    FindProtostems();
    CreateStemAffixPairs();
    AssignSuffixesToStems();
    ComputeMultiparses();
    SortMultiparses(1);
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
    QMap<QString,CWord*>* WordMap = m_Words->GetMap();
    QMapIterator<QString,CWord*> word_iter(*WordMap);
    while (word_iter.hasNext())   {
        word = word_iter.next().value()->GetWord();
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
    QString stem, affix, signature_string;
    CStem* pStem;
    QMap<QString,QSet<QString>*> temp_stems_to_affixes;
       
    for (int parseno = 0; parseno < m_Parses->size(); parseno++){
        this_pair = m_Parses->at(parseno);
        stem = this_pair.first;
        affix = this_pair.second;

        if (! temp_stems_to_affixes.contains(stem)){
            QSet<QString> * pSet = new QSet<QString>;
            temp_stems_to_affixes.insert(stem,pSet);
        }

        temp_stems_to_affixes.value(stem)->insert(affix);
    }
    // Do we need to delete the QSets, or is their memory taken care of? Probably need to delete them...and below.

    QMap<QString,QSet<QString>*>::iterator stem_iter;
    QMap<QString,QSet<QString>*> temp_signatures_to_stems;
    for (stem_iter = temp_stems_to_affixes.begin(); stem_iter != temp_stems_to_affixes.end(); ++ stem_iter) {
         stem = stem_iter.key();
         QStringList temp_presignature;
         foreach (affix, *temp_stems_to_affixes.value(stem)){
            temp_presignature.append(affix);           
        }
        temp_presignature.sort();
        QString signature = temp_presignature.join("=");
        if ( ! temp_signatures_to_stems.contains(signature)){
            QSet<QString> * pStemSet = new QSet<QString>;
            temp_signatures_to_stems.insert(signature,pStemSet);
        }
        temp_signatures_to_stems.value(signature)->insert(stem);
    }

    // create signatures, stems, affixes:
    QMap<QString,QSet<QString>*>::iterator signatures_iter;
    for (signatures_iter = temp_signatures_to_stems.begin(); signatures_iter != temp_signatures_to_stems.end(); ++ signatures_iter ){
        if (signatures_iter.value()->size() >= MINIMUM_NUMBER_OF_STEMS){
            signature_string = signatures_iter.key();
            QList<QString> affixes = signature_string.split("=");
            QListIterator<QString> affix_iter(affixes);
            while(affix_iter.hasNext()){
                  affix = affix_iter.next();
                  m_Suffixes->find_or_add(affix);                   
            }

            pSig = new CSignature(signature_string);
            *m_Signatures<< pSig;
            foreach (stem, *signatures_iter.value()){
                pStem = m_Stems->find_or_add(stem);
                pStem->add_signature (signature_string);
                pSig->add_stem_pointer(pStem);
                QStringList affixes = signature_string.split("=");
                QString word;
                foreach (QString affix,  affixes){
                    if (affix == "NULL"){
                        word = stem;
                    }
                    else{
                        word = stem + affix;
                    }
                    CWord* pWord = m_Words->get_word(word);
                    pWord->add_stem_and_signature(pStem,pSig);
                }
            }
        }
    }
}

typedef  QPair<CStem*,CSignature*>  stem_sig_pair;
typedef  QPair<stem_sig_pair*,  stem_sig_pair*> pair_of_stem_sig_pairs;
typedef  QPair<QString, pair_of_stem_sig_pairs*> five_tuple_sig_diffs;
void CLexicon::ComputeMultiparses()
{
    CWord* pWord;
    QMap<QString,CWord*>* WordMap = m_Words->GetMap();
    QMapIterator<QString,CWord*> word_iter(*WordMap);
    const five_tuple_sig_diffs *  five_tuple;

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
                    m_Multiparses.append( *five_tuple);
                }
            }
        }
    }
}

void CLexicon::SortMultiparses(int styleno){

    const five_tuple_sig_diffs * multiparse;
    pair_of_stem_sig_pairs * this_pair_of_stem_sig_pairs;
    stem_sig_pair * stem_sig_pair_1, * stem_sig_pair_2;
    int mp_no;
    QString difference;
    QString stem1, stem2;
    QString sig1, sig2;
    if (styleno == 1) {
    for (mp_no = 0; mp_no < m_Multiparses.size(); mp_no++){
        multiparse = & m_Multiparses.at(mp_no);
        difference = multiparse->first;
        this_pair_of_stem_sig_pairs = multiparse->second;
        stem_sig_pair_1 = this_pair_of_stem_sig_pairs->first;
        stem_sig_pair_2 = this_pair_of_stem_sig_pairs->second;
        stem1 = stem_sig_pair_1->first->get_key();
        sig1 = stem_sig_pair_1->second->get_key();
        stem2 = stem_sig_pair_2->first->get_key();
        sig2 = stem_sig_pair_2->second->get_key();
        qDebug() << "**" << difference << stem1  << sig1 << stem2<< sig2;
      }
    }
}
























