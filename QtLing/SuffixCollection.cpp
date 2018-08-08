#include "SuffixCollection.h"
#include "Lexicon.h"
#include <QDebug>
#include <QJsonArray>

class CLexicon;

CSuffixCollection::CSuffixCollection(CLexicon* lexicon)
{
    m_SuffixMap = QMap<QString, CSuffix*>();
    m_Lexicon  = lexicon;
}
CSuffixCollection::~CSuffixCollection() {

}

void CSuffixCollection::clear(){
    m_SuffixMap.clear();
    m_SortedList.clear();
}

CSuffix* CSuffixCollection::operator ^=(const QString& suffix)
{
      return this->find_or_fail ( suffix );
}
CSuffix* CSuffixCollection::find_or_fail(const QString& suffix)
{
    QMap<QString,CSuffix*>::iterator suffix_iter = m_SuffixMap.find(suffix);
    //map_string_to_suffix_iter suffix_iter = m_SuffixMap.find(suffix);
    if (suffix_iter == m_SuffixMap.end()){
        return NULL;
    } else{
        return suffix_iter.value();
    }

}
CSuffix* CSuffixCollection::find_or_add(const QString& suffix)
{

    QMap<QString,CSuffix*>::iterator suffix_iter = m_SuffixMap.find(suffix);

    if (suffix_iter == m_SuffixMap.end()){
      CSuffix* pSuffix = new CSuffix(suffix);
      m_SuffixMap.insert(suffix, pSuffix);
      return pSuffix;
    }
    return suffix_iter.value();
}


CSuffix* CSuffixCollection::operator << (const QString& suffix)
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

bool count_compare_suffixes(CSuffix* pSuff1, CSuffix* pSuff2){
    if (pSuff1->get_sig_count() > pSuff2->get_sig_count()) return true;
    return false;
}

void CSuffixCollection::sort_by_count()
{
    QMapIterator<QString, CSuffix*> suffix_iter (m_SuffixMap);
    while (suffix_iter.hasNext()){
        suffix_iter.next();
        m_SortedList.append(suffix_iter.value());
    }
    std::sort(m_SortedList.begin(), m_SortedList.end(), count_compare_suffixes);

}







CPrefixCollection::CPrefixCollection(CLexicon* lexicon)
{
    m_PrefixMap = QMap<QString, CPrefix*>();
    m_Lexicon   = lexicon;
}
CPrefixCollection::~CPrefixCollection() {

}
void CPrefixCollection::clear(){
    m_PrefixMap.clear();
    m_SortedList.clear();
}

CPrefix* CPrefixCollection::operator ^=(const QString& prefix)
{
      return this->find_or_fail ( prefix );
}
CPrefix* CPrefixCollection::find_or_fail(const QString& prefix)
{
    QMap<QString,CPrefix*>::iterator prefix_iter = m_PrefixMap.find(prefix);
    //map_string_to_suffix_iter suffix_iter = m_SuffixMap.find(suffix);
    if (prefix_iter == m_PrefixMap.end()){
        return NULL;
    } else{
        return prefix_iter.value();
    }

}
CPrefix* CPrefixCollection::find_or_add(const QString& prefix)
{

    QMap<QString,CPrefix*>::iterator prefix_iter = m_PrefixMap.find(prefix);

    if (prefix_iter == m_PrefixMap.end()){
      CPrefix* pPrefix = new CPrefix(prefix);
      m_PrefixMap.insert(prefix, pPrefix);
      return pPrefix;
    }
    return prefix_iter.value();
}



CPrefix* CPrefixCollection::operator << (const QString& prefix)
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
bool count_compare_prefixes(CPrefix* pPrefix1, CPrefix* pPrefix2){
    if (pPrefix1->get_sig_count() > pPrefix2->get_sig_count()) return true;
    return false;
}

void CPrefixCollection::sort_by_count()
{
    QMapIterator<QString, CPrefix*> prefix_iter (m_PrefixMap);
    while (prefix_iter.hasNext()){
        prefix_iter.next();
        m_SortedList.append(prefix_iter.value());
    }
    std::sort(m_SortedList.begin(), m_SortedList.end(), count_compare_prefixes);

}

