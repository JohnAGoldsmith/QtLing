#include "WordCollection.h"

#include <QDebug>
#include "Word.h"

CWordCollection::CWordCollection(CLexicon * lexicon)
{
    m_CorpusCount			= 0;
    m_MemberName			= QString();
    m_SortValidFlag			= 0;
    m_SortStyle				= KEY;
    m_Lexicon               = lexicon;
}

CWordCollection::~CWordCollection()
{
    if ( m_SortedStringArray.size() > 0 )         {  m_SortedStringArray.empty();  }
}

QMapIterator<QString,CWord*> *  CWordCollection::get_iterator(){
    QMapIterator<QString, CWord*> * iter = new QMapIterator<QString,CWord*>(m_WordMap);
    return iter;
}

CWord* CWordCollection::get_word(const QString& word){
    QMap<QString,CWord*>::const_iterator word_iter = m_WordMap.find(word);
    if (word_iter == m_WordMap.end()){
        return NULL;
    }
    return word_iter.value();
}
CWord* CWordCollection::get_word(const int n){
    QString word = get_string_from_sorted_list(n);
    return get_word(word);
}

CWord* CWordCollection::find_or_add(const QString& word_string){
    QMap<QString,CWord*>::const_iterator word_iter = m_WordMap.find(word_string);
    if (word_iter == m_WordMap.end()){
        CWord* pWord = new CWord(word_string);
        m_WordMap[word_string] = pWord;
        return pWord;
    }else{
        return word_iter.value();
    }
}
CWord* CWordCollection::find_or_fail(const QString& word_string){
    QMap<QString,CWord*>::const_iterator word_iter = m_WordMap.find(word_string);
    //qDebug() << word_string;
    if (word_iter == m_WordMap.end()){
         //qDebug() << word_string + " not found in find or fail";
         return NULL;
    }else{
        //qDebug() << "found something in find or fail";
        return word_iter.value();
    }
}
CWord* CWordCollection::add(const QString& word)
{
    CWord* pWord = new CWord(word);
    m_WordMap[word] = pWord;
    return pWord;
}

CWord* CWordCollection::operator <<(const QString& word)
{
     return this->add(word);
}


CWord* CWordCollection::operator ^=(const QString& word)
{
    return this->find_or_add(word);


}



//-->  Reverse string sorting comparator <--//
// -->  returns true if string1 precedes string2 in reverse alphabetical order  <-- //
// --> July 2018 -- this had a bug regarding strings of different lengths.
bool reverse_string_compare(QString string1, QString string2){
    if (string1.length() == 0) {return true;}
    if (string2.length() == 0) {return false;}
    int len1 = string1.length();
    int len2 = string2.length();
    int limit = std::min(len1,len2);
    int i = 1;
    while ( i <= limit && string1[len1-i] == string2[len2-i] )
    {
        i++;
    }
    if ( i <= limit){
       if (string1[len1-i] != string2[len2-i])
       {
            return string1[len1-i]  < string2[len2-i];
       }
    }

    if (len1 < len2){
        return true;
    }
    if (len2 < len1){
        return false;
    }
    if (string1 == string2 ){
        return false;
    }
    qDebug() <<"Reverse string compare, problem here." << string1 << string2;

    return false;
}

bool new_reverse_string_compare(QString s1, QString s2)
{
    std::reverse(s1.begin(), s1.end());
    std::reverse(s2.begin(), s2.end());
    return s1 < s2;
}

void CWordCollection::sort_word_list()
{   m_SortedStringArray.clear();

    foreach(QString word, m_WordMap.keys()){
        m_SortedStringArray.append(word);
    }
    m_SortedStringArray.sort();

    m_reverse_sort_list.reserve(m_WordMap.size());
    foreach(QString word, m_WordMap.keys()){
        m_reverse_sort_list.append(word);
    }
    std::sort(m_reverse_sort_list.begin(),m_reverse_sort_list.end(), new_reverse_string_compare);

}
