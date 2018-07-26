#include "Word.h"
#include <QDebug>
CWord::CWord(QString word) :  m_Word(word), m_WordCount(0)
{
    m_Signatures.clear();
}

CWord::CWord(CWord& word)
{
    m_WordCount = word.get_word_count();
    m_Word      = word.get_word();
    m_Signatures.clear();
 }


CWord::~CWord()
{
    clear_parse_triple_map();
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

void CWord::add_parse_triple(QString stem, QString affix, QString sig_string)
{
    Parse_triple * this_triple = new Parse_triple(stem, affix, sig_string);
   // m_Parse_triples.append(this_triple);
    m_Parse_triple_map[stem]= this_triple;

}

void CWord::clear_parse_triple_map()
{
    QMap<stem_t, Parse_triple*>::iterator ptm_iter;
    for (ptm_iter = m_Parse_triple_map.begin();
         ptm_iter != m_Parse_triple_map.end();
         ptm_iter++) {
        delete ptm_iter.value();
    }
}
