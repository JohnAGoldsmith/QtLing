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
CWord* CWordCollection::find_or_fail(QString word_string){
    QMap<QString,CWord*>::const_iterator word_iter = m_WordMap.find(word_string);
    //qDebug() << word_string;
    if (word_iter == m_WordMap.end()){
       // qDebug() << "not found in find or fail";
         return NULL;
    }else{
        //qDebug() << "found something in find or fail";
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



//-->  Reverse string sorting comparator <--//
// -->  returns true if string1 precedes string2 in reverse alphabetical order  <-- //
bool reverse_string_compare(QString string1, QString string2){    if (string1.length() == 0) {return true;}
    if (string2.length() == 0) {return false;}
    int len1 = string1.length();
    int len2 = string2.length();
    int limit = std::min(len1,len2);
    for (int i = 0; i < limit; i++){
        if (string1[len1-i]== string2[len2-i]){
            continue;
        }
        return string1[len1-i] < string2[len2-i];
    }
    // i now equals limit
    if (len1 == len2){
        return false ; //the two words are identical, however.
    }
    if (len1 < len2){
        return true;
    }
    return false;
}



void CWordCollection::sort_word_list()
{
    foreach(QString word, m_WordMap.keys()){
        m_SortedStringArray.append(word);
    }
    m_SortedStringArray.sort();
    m_reverse_sort_list.reserve(m_WordMap.size());
    foreach(QString word, m_WordMap.keys()){
        m_reverse_sort_list.append(word);
    }
    std::sort(m_reverse_sort_list.begin(),m_reverse_sort_list.end(), reverse_string_compare);

}
