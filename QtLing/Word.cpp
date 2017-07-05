#include "Word.h"

CWord::CWord(CStringSurrogate ssWord) : CParse(ssWord)
{
    m_WordCount = 0;
    m_Word = QString(ssWord.GetKey());
}
CWord::CWord(CWord& word)
{
    m_WordCount = word.GetWordCount();
    m_Word      = word.GetWord();
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

