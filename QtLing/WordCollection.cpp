#include "WordCollection.h"

#include "Parse.h"
#include "Word.h"

CWordCollection::CWordCollection()
{
    m_WordList              = QList<CWord>();
    m_CorpusCount			= 0;
    m_MemberName			= QString::null;
    m_SortArray				= NULL;
    m_SortValidFlag			= 0;
    m_SortStyle				= KEY;
}
//CWordCollection::CWordCollection(/*CLexicon* Lex*/ QString MemberName)
//{
//    m_WordList              = QList<CWord>();
//    m_CorpusCount			= 0;
//    m_MemberName			= QString::null;
//    m_SortArray				= NULL;
//    m_SortValidFlag			= 0;
//    m_SortStyle				= KEY;
//}
CWordCollection::~CWordCollection()
{
    if ( m_SortArray )         { delete [] m_SortArray; m_SortArray = NULL;  }
}
CWord* CWordCollection::operator<<( CParse* pParse )
// Good for everything except CSignatures, which need punctuation.
{
    CStringSurrogate  cssKey;
    cssKey = pParse->GetKey();

    CWord* word = new CWord(cssKey);
    word->IncrementWordCount();
    m_WordList << *word;
    return word;
}
CWord* CWordCollection::operator <<(CStringSurrogate SS)
{
    CWord* word = new CWord(SS);
    word->IncrementWordCount();
    m_WordList << *word;
    return word;
}

CWord* CWordCollection::operator <<(QString szWord)
{
    CStringSurrogate cssKey = CStringSurrogate(szWord, szWord.length());
//    CWord* word = new CWord(cssKey);
//    m_WordList << *word;
//    return word;
    CWord word = CWord(cssKey);
    m_WordList << word;
    return &word;
}
CWord* CWordCollection::operator ^=(CParse* string)
{
    CStringSurrogate cssKey = string->GetKey();

    CWord* pWord = new CWord(cssKey);
    int length = m_WordList.length();
    for (int i = 0 ; i < length; i++) {
        if (pWord->GetWord() == m_WordList.at(i).GetWord()) {
            pWord->IncrementWordCount();
            return pWord;
        }
    }
    return 0;
}
CWord* CWordCollection::operator ^=(CStringSurrogate string)
{
    CWord* pWord = new CWord(string);
    int length = m_WordList.length();
    for (int i = 0 ; i < length; i++) {
        if (pWord->GetWord() == m_WordList.at(i).GetWord()) {
            pWord->IncrementWordCount();
            return pWord;
        }
    }
    return 0;
}
CWord* CWordCollection::operator ^=(QString szWord)
{
    CStringSurrogate cssKey = CStringSurrogate(szWord, szWord.length());
    CWord* pWord = new CWord(cssKey);

    int length = m_WordList.length();
    for (int i = 0 ; i < length; i++) {
        if (pWord->GetWord() == m_WordList.at(i).GetWord()) {
            pWord->IncrementWordCount();
            return pWord;
        }
    }
    return 0;

}
CWord CWordCollection::GetAt( uint n )
{
    return m_WordList.at(n);
}
