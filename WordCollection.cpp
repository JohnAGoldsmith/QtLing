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
    if ( m_sorted_list.size() > 0 )         {  m_sorted_list.empty();  }
}
//--------------------//
void CWordCollection::input_words(QMap<QString, int> word_counts){
    for(auto i = word_counts.begin(), end = word_counts.end(); i != end; i++){
        add_word(i.key(), i.value());
    }
    sort_word_lists();
}
bool new_reverse_string_compare(QString s1, QString s2)
{
    std::reverse(s1.begin(), s1.end());
    std::reverse(s2.begin(), s2.end());
    return s1 < s2;
}
void CWordCollection::sort_word_lists()
{   m_sorted_list.clear();
    m_end_sorted_list.clear();
    foreach(QString word, m_WordMap.keys()){
        m_sorted_list.append(word);
    }
    m_sorted_list.sort();

    m_end_sorted_list.reserve(m_WordMap.size());
    foreach(QString word, m_WordMap.keys()){
        m_end_sorted_list.append(word);
    }
    std::sort(m_end_sorted_list.begin(),m_end_sorted_list.end(), new_reverse_string_compare);
}
//--------------------//

void  CWordCollection::clear_all_words_parse_triple_maps(){
    for (auto i = m_WordMap.cbegin(), end = m_WordMap.cend(); i != end; ++i  ){
        i.value()->clear_parse_triple_map();
    }
}


QMapIterator<QString,CWord*> *  CWordCollection::get_iterator(){
    QMapIterator<QString, CWord*> * iter = new QMapIterator<QString,CWord*>(m_WordMap);
    return iter;
}
int CWordCollection::get_token_count(){
    int total = 0;
    QMapIterator<QString, CWord* > iter(m_WordMap);
    while(iter.hasNext())
    {
        iter.next();
        total += iter.value()->get_word_count();
        //qDebug() << iter.key() << " : " << iter.value();
    }
    qDebug() << total;
    return total;
}
CWord* CWordCollection::get_word(const QString& word){
    if (m_WordMap.contains(word)){
        return m_WordMap[word];
    } else{
        return NULL;
    }
}
CWord* CWordCollection::get_word(const int n){
    return m_word_list[n];
}
int CWordCollection::get_word_count(){
    return m_WordMap.count();
}


CWord* CWordCollection::find_or_add(const QString& word_string){
    QMap<QString,CWord*>::const_iterator word_iter = m_WordMap.find(word_string);
    if (word_iter == m_WordMap.end()){
        CWord* pWord = new CWord(word_string);
        m_WordMap[word_string] = pWord;
        m_word_list.append(pWord);
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
CWord* CWordCollection::add_word(const QString& word, int count)
{   CWord* pWord;
    if (m_WordMap.contains(word)){
        m_WordMap[word]->SetWordCount(count);
    } else {
        pWord = new CWord(word);
        pWord->SetWordCount(count);
        m_WordMap[word] = pWord;
        m_word_list.append(pWord);
    }
    return pWord;
}

CWord* CWordCollection::operator <<(const QString& word)
{
    return this->add_word(word);
}


CWord* CWordCollection::operator ^=(const QString& word)
{
    return this->find_or_add(word);
}




