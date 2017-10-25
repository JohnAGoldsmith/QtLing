#include "WordCollection.h"

#include <QDebug>
#include "Word.h"

CWordCollection::CWordCollection()
{
    m_CorpusCount			= 0;
    m_MemberName			= QString::null;
    m_SortValidFlag			= 0;
    m_SortStyle				= KEY;
}

CWordCollection::~CWordCollection()
{
    if ( m_SortedStringArray.size() > 0 )         {  m_SortedStringArray.empty();  }
}

QMapIterator<QString,CWord*> *  CWordCollection::get_iterator(){
    QMapIterator<QString, CWord*> * iter = new QMapIterator<QString,CWord*>(m_WordMap);
    return iter;
}

CWord* CWordCollection::get_word(QString word){
    QMap<QString,CWord*>::const_iterator word_iter = m_WordMap.find(word);
    if (word_iter == m_WordMap.end()){
        return NULL;
    }
    return word_iter.value();
}

CWord* CWordCollection::find_or_add(QString word_string){
    QMap<QString,CWord*>::const_iterator word_iter = m_WordMap.find(word_string);
    if (word_iter == m_WordMap.end()){
        CWord* pWord = new CWord(word_string);
        m_WordMap[word_string] = pWord;
    }else{
        return word_iter.value();
    }
}
CWord* CWordCollection::find_or_null(QString word_string){
    QMap<QString,CWord*>::const_iterator word_iter = m_WordMap.find(word_string);
    if (word_iter == m_WordMap.end()){
         return NULL;
    }else{
        return word_iter.value();
    }
}
CWord* CWordCollection::add(QString word)
{
    CWord* pWord = new CWord(word);
    m_WordMap[word] = pWord;
    return pWord;
}

CWord* CWordCollection::operator <<(QString word)
{
     return this->add(word);
}


CWord* CWordCollection::operator ^=(QString word)
{
    this->find_or_add(word);

}

void CWordCollection::sort_word_list()
{
    foreach(QString word, m_WordMap.keys()){
        m_SortedStringArray.append(word);
    }
    m_SortedStringArray.sort();

}
