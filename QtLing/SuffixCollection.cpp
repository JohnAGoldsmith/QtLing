#include "SuffixCollection.h"
#include <QDebug>
//#include "Parse.h"
//#include "Suffix.h"

CSuffixCollection::CSuffixCollection()
{
    m_SuffixMap = QMap<QString, CSuffix*>();
}
CSuffixCollection::~CSuffixCollection() {

}


CSuffix* CSuffixCollection::operator ^=(QString suffix)
{
      return this->find_or_fail ( suffix );
}
CSuffix* CSuffixCollection::find_or_fail(QString suffix)
{
    QMap<QString,CSuffix*>::iterator suffix_iter = m_SuffixMap.find(suffix);
    //map_string_to_suffix_iter suffix_iter = m_SuffixMap.find(suffix);
    if (suffix_iter == m_SuffixMap.end()){
        return NULL;
    } else{
        return suffix_iter.value();
    }

}
CSuffix* CSuffixCollection::find_or_add(QString suffix)
{

    QMap<QString,CSuffix*>::iterator suffix_iter = m_SuffixMap.find(suffix);

    if (suffix_iter == m_SuffixMap.end()){
      CSuffix* pSuffix = new CSuffix(suffix);
      m_SuffixMap.insert(suffix, pSuffix);
      return pSuffix;
    }
    return suffix_iter.value();
}


CSuffix* CSuffixCollection::operator << (QString suffix)
{
    return this->find_or_add(suffix);
}

void CSuffixCollection::get_set_of_suffixes(QSet<QString> *p_string_set){
    p_string_set->fromList(m_SuffixMap.uniqueKeys());
}

void  CSuffixCollection::get_suffixes(QList<QString>* pList)
{   QMapIterator<QString,CSuffix*> iter (m_SuffixMap);
    while (iter.hasNext()){
        iter.next();
        pList->append(iter.value()->get_key());
    }
}













CPrefixCollection::CPrefixCollection()
{
    m_PrefixMap = QMap<QString, CPrefix*>();
}
CPrefixCollection::~CPrefixCollection() {

}


CPrefix* CPrefixCollection::operator ^=(QString suffix)
{
      return this->find_or_fail ( suffix );
}
CPrefix* CPrefixCollection::find_or_fail(QString suffix)
{
    QMap<QString,CPrefix*>::iterator prefix_iter = m_PrefixMap.find(suffix);
    //map_string_to_suffix_iter suffix_iter = m_SuffixMap.find(suffix);
    if (prefix_iter == m_PrefixMap.end()){
        return NULL;
    } else{
        return prefix_iter.value();
    }

}
CPrefix* CPrefixCollection::find_or_add(QString prefix)
{

    QMap<QString,CPrefix*>::iterator prefix_iter = m_PrefixMap.find(prefix);

    if (prefix_iter == m_PrefixMap.end()){
      CPrefix* pPrefix = new CPrefix(prefix);
      m_PrefixMap.insert(prefix, pPrefix);
      return pPrefix;
    }
    return prefix_iter.value();
}


CPrefix* CPrefixCollection::operator << (QString prefix)
{
    return this->find_or_add(prefix);
}

void CPrefixCollection::get_set_of_prefixes(QSet<QString> *p_string_set){
    p_string_set->fromList(m_PrefixMap.uniqueKeys());
}

void  CPrefixCollection::get_prefixes(QList<QString>* pList)
{   QMapIterator<QString,CPrefix*> iter (m_PrefixMap);
    while (iter.hasNext()){
        iter.next();
        pList->append(iter.value()->get_key());
    }
}
