#include "StemCollection.h"
#include <QDebug>
#include <QMapIterator>

CStemCollection::CStemCollection(CLexicon* lexicon)
{
    m_StringToStemMap       = new map_string_to_stem();
    m_CorpusCount			= 0;
    m_MemberName			= QString::null;
    m_SortValidFlag			= 0;
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
    m_SortList.clear();
    m_CorpusCount = 0;
}
CStem* CStemCollection::add(const QString& stem)
{
    CStem* pStem = new CStem(stem);
    m_StringToStemMap->insert(stem, pStem);
    return pStem;
}
CStem* CStemCollection::operator <<(const QString& stem)
{
    return this->add(stem);
}

CStem* CStemCollection::operator ^=(const QString& stem)
{ return this->find_or_add(stem);
}


CStem* CStemCollection::GetAtKey( QString stem)
{
    return m_StringToStemMap->value(stem);
}


QListIterator<CStem*> * CStemCollection::get_sorted_list_iterator()
{
    QListIterator<CStem*> * iter = new QListIterator<CStem*>(m_SortList);
    return iter;
}
CStem* CStemCollection::find_or_add(const QString& stem)
{
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
