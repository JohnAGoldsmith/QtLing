#include "Word.h"
#include <QDebug>
CWord::CWord(QString word) : m_WordCount(0), m_Word(word)
{
     m_WordCount = 0;
    m_Signatures.clear();
}

CWord::CWord(CWord& word)
{
    m_WordCount = word.get_word_count();
    m_Word      = word.GetWord();
    m_Signatures.clear();
 }

// Increment the word count
//
// Parameters:
//    n - amount to increment, default = 1

void CWord::IncrementWordCount (int n )
{
  m_WordCount += n;
  Q_ASSERT (m_WordCount > 0);
  Q_ASSERT (m_WordCount < 1000000);
}

void CWord::AddParse(CStem* stem, CSuffix* suffix, CSignature* signature)
{
    Parse parse;
    parse.p_stem = stem;
    parse.p_suffix = suffix;
    parse.p_signature = signature;
    m_Parses << parse;
}

void CWord::add_stem_and_signature(CStem* pStem, CSignature * pSig)
{   //Debug() << "word 38" << get_key() << "size"<<m_Signatures.size();
    for (int i =0; i< m_Signatures.size(); i++){
        ptr_to_stem_sig_pair stem_ptr = m_Signatures.at(i);
        if (stem_ptr->first->get_key() == pStem->get_key()){
            return;
        }
    }
    QPair<CStem*,CSignature*>* pPair = new QPair<CStem*,CSignature*>(pStem,pSig);
    m_Signatures.append(pPair);

}
