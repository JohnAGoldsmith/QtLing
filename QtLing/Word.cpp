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
void CWord::add_parse_triple(QString stem, QString affix, QString sig_string)
{
    Parse_triple * this_triple = new Parse_triple(stem, affix, sig_string);
   // m_Parse_triples.append(this_triple);
    m_Parse_triple_map[stem]= this_triple;

}

void CWord::clear_parse_triple_map()
{
    foreach (Parse_triple* p_parse_triple, m_Parse_triple_map) {
        delete p_parse_triple;
    }
    m_Parse_triple_map.clear();
} // simplified and modified by Hanson, 7.31

bool CWord::contains_this_stem_among_parses(QString stem){
    return m_Parse_triple_map.contains(stem);
}
void CWord::add_morphemic_split(QString split){
    if (m_Morphemic_splits.contains(split)){
        return;
    }
    m_Morphemic_splits.append(split);
    return;
}
void CWord::remove_morphemic_split(QString morpheme_split){
    if (m_Morphemic_splits.contains(morpheme_split)){
        m_Morphemic_splits.removeAll(morpheme_split);
    }
}
