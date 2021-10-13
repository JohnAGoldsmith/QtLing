#include "StemCollection.h"
#include <QDebug>
#include <QMapIterator>

CStemCollection::CStemCollection(CLexicon* lexicon)
{
    m_StringToStemMap       = new map_string_to_stem();
    m_CorpusCount			= 0;
    m_MemberName			= QString();
    m_forward_sort_valid_flag			= 0;
    m_SortStyle				= KEY;
    m_Lexicon               = lexicon;
}


CStemCollection::~CStemCollection()
{
    // delete each stem individually, and then...
    // Moved some code here into clear(): -- Hanson 7.31
    clear();
    delete m_StringToStemMap;
}

void CStemCollection::clear(){
    map_string_to_stem_ptr_iter iter (*m_StringToStemMap);
    while (iter.hasNext()){
        iter.next();
        delete iter.value();
    }
    /*
    for (int i = 0; i < m_SortList.size(); i++){
     delete m_SortList.at(i);
    }
    */
    m_StringToStemMap->clear();
    m_SortedStringList.clear();
    m_CorpusCount = 0;
}
CStem* CStemCollection::add(const QString& stem)
{
    CStem* pStem = new CStem(stem);
    m_StringToStemMap->insert(stem, pStem);
    m_forward_sort_valid_flag = false;
    return pStem;
}
CStem* CStemCollection::operator <<(const QString& stem)
{
    return this->add(stem);
}

CStem* CStemCollection::operator ^=(const QString& stem)
{
    return this->find_or_add(stem);
}


CStem* CStemCollection::get_from_string( QString stem)
{
    return m_StringToStemMap->value(stem);
}

CStem* CStemCollection::get_at( int stemno)
{
    if (!m_forward_sort_valid_flag){ sort_alphabetically();}
    return m_StringToStemMap->value( m_SortedStringList[stemno]);
}
/*
QListIterator<CStem*> * CStemCollection::get_sorted_list_iterator()
{
    if (!m_SortValidFlag    ){
        sort_alphabetically();
    }
    QListIterator<CStem*> * iter = new QListIterator<CStem*>(m_SortedStringList);
    return iter;
}
*/
CStem* CStemCollection::find_or_add(const QString& stem)
{
    if (!m_forward_sort_valid_flag){
        sort_alphabetically();
    }
    if (m_StringToStemMap->contains(stem)){
        return m_StringToStemMap->value(stem);
    } else{
        CStem* pStem = new CStem(stem);
        m_StringToStemMap->insert(stem, pStem);
        return pStem;
    }
}
CStem* CStemCollection::find_or_fail(const QString& stem)
{
    if (m_StringToStemMap->contains(stem)){
        return m_StringToStemMap->value(stem);
    } else{
        return NULL;
    }
}
void CStemCollection::sort_alphabetically()
{   m_SortedStringList.clear();
    foreach(QString word, m_StringToStemMap->keys()){
        m_SortedStringList.append(word);
    }

    m_SortedStringList.sort();
    m_forward_sort_valid_flag = true;
}

QString CStemCollection::get_string_at(int index) {
    if (!m_forward_sort_valid_flag) { sort_alphabetically();}
        return m_SortedStringList[index];
}
