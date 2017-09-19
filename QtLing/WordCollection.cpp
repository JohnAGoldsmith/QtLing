#include "WordCollection.h"

#include <QDebug>
#include "Parse.h"
#include "Word.h"

CWordCollection::CWordCollection()
{
    m_WordList              = QList<CWord*>();
    m_CorpusCount			= 0;
    m_MemberName			= QString::null;
    //m_SortArray				= NULL;
    m_SortValidFlag			= 0;
    m_SortStyle				= KEY;
}
//CWordCollection::CWordCollection(CLexicon* Lex, QString MemberName)
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
    if ( m_SortedStringArray.size() > 0 )         {  m_SortedStringArray.empty();  }
}

CWord* CWordCollection::add(QString word_string){
    *this << word_string;
}

CWord* CWordCollection::operator <<(QString szWord)
{
    qDebug() << szWord << " line 36 in word colletion";
    CWord* pWord = new CWord(szWord);
    m_WordList << pWord;
    m_WordMap[szWord] = pWord;
    return pWord;
}
CWord* CWordCollection::operator ^=(CParse* string)
{
    CStringSurrogate cssKey = string->GetKey();

    CWord* pWord = new CWord(cssKey);
    int length = m_WordList.length();
    for (int i = 0 ; i < length; i++) {
        if (pWord->GetWord() == m_WordList.at(i)->GetWord()) {
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
        if (pWord->GetWord() == m_WordList.at(i)->GetWord()) {
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
        if (pWord->GetWord() == m_WordList.at(i)->GetWord()) {
            pWord->IncrementWordCount();
            return pWord;
        }
    }
    return 0;
}

void CWordCollection::sort_word_list()
{
    foreach(QString word, m_WordMap.keys()){
        m_SortedStringArray.append(word);
    }
    m_SortedStringArray.sort();

}
