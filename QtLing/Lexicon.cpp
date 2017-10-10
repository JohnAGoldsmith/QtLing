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
//#include "StringSurrogate.h"
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


void CLexicon::ComputeMultiparses()
{
    CWord* pWord;
    QMap<QString,CWord*>* WordMap = m_Words->GetMap();
    QMapIterator<QString,CWord*> word_iter(*WordMap);
    while (word_iter.hasNext())   {
        pWord = word_iter.next().value();
        if (pWord->GetSignatures()->size() > 2){

        }




    }


}

























