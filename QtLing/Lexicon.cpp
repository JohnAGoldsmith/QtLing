#include <QPair>
#include <QList>
#include <QMap>
#include <QMultiMap>

#include <QtDebug>

#include "Lexicon.h"

#include "SignatureCollection.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "WordCollection.h"
#include "StringSurrogate.h"
#include "Word.h"

CLexicon::CLexicon() : m_Words(new CWordCollection), m_Stems(new CStemCollection), m_Suffixes(new CSuffixCollection), m_Signatures(new CSignatureCollection)
{
    m_Parses = new QList<QPair<QString,QString>>();
    m_Protostems = QMap<CStringSurrogate, int>();
}

//linguistic methods

void CLexicon::Crab_1()
{
    FindProtostems();
    CreateStemAffixPairs();
    
    qDebug() << "Hello world";
    
    //AssignSuffixesToStems();
}

void CLexicon::FindProtostems()
{  QString word, previous_word;
    CWordCollection & Words =  * m_Words;
    bool StartFlag = true;
    bool DifferenceFoundFlag = false;
    QString stem;
    for (int wordno=0; wordno<Words.GetLength(); wordno ++){
        word = Words.GetAt(wordno)->GetWord();
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
    for (int wordno=0; wordno<m_Words->GetLength(); wordno ++){
        word = m_Words->GetAt(wordno)->GetWord();
        
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
    QMultiMap<QString,QString> temp_stems_to_affixes;
    

    
    for (int parseno = 0; parseno < m_Parses->size(); parseno++){
        this_pair = m_Parses->at(parseno);
        stem = this_pair.first;
        affix = this_pair.second;
        temp_stems_to_affixes.insert(stem,affix);
    }
    
    QMultiMap<QString,QString>::const_iterator stem_iter;
    QMultiMap<QString,QString> temp_signatures_to_stems;
    for (stem_iter = temp_stems_to_affixes.constBegin(); stem_iter != temp_stems_to_affixes.constEnd(); ++ stem_iter) {
        stem = stem_iter.key();
        QStringList affixes = temp_stems_to_affixes.values(stem);
        affixes.sort();
        signature_string = affixes.join("=");
        temp_signatures_to_stems.insertMulti(signature_string,stem);
    }
    
    QMultiMap<QString,QString>::const_iterator signatures_iter;
    for (signatures_iter = temp_signatures_to_stems.constBegin(); signatures_iter != temp_signatures_to_stems.constEnd(); ++ signatures_iter ){
        if (signatures_iter.value().length() >= MINIMUM_NUMBER_OF_STEMS){
            pSig = m_Signatures->find(signature_string);
            foreach (stem, temp_signatures_to_stems.values(signature_string)) {
                pStem = m_Stems->find_stem(stem);
                pSig->add_stem_pointer(pStem);
            }
        }
    }
}




























