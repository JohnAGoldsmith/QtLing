#include "Word.h"
#include <QDebug>
CWord::CWord(QString word) :  m_Word(word), m_WordCount(0)
{
    m_Signatures.clear();
}

CWord::CWord(CWord& word)
{
    m_WordCount = word.get_word_count();
    m_Word      = word.GetWord();
    m_Signatures.clear();
 }


CWord::~CWord(){

}

void CWord::IncrementWordCount (int n )
{
  m_WordCount += n;
  Q_ASSERT (m_WordCount > 0);
  Q_ASSERT (m_WordCount < 1000000);
}
void CWord::add_to_autobiography(QString line)
{
     m_Autobiography.append(line);
}
 void CWord::add_parse_triple(QString stem, QString affix, CSignature* pSig)
{
    Parse_triple * this_triple = new Parse_triple(stem, affix, pSig, pSig->get_key());
    m_Parse_triples.append(this_triple);

}
