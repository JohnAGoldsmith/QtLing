#include "SuffixCollection.h"
#include "Lexicon.h"
#include <QDebug>
#include <QJsonArray>

class CLexicon;

CSuffixCollection::CSuffixCollection(CLexicon* lexicon)
{
    m_suffix_map = QMap<QString, CSuffix*>();
    m_Lexicon  = lexicon;
}
CSuffixCollection::~CSuffixCollection() {

}

void CSuffixCollection::clear(){
    foreach (CSuffix* suffix, m_suffix_list){
        //delete suffix;
    }
    m_suffix_map.clear();
    m_suffix_list.clear();
}

CSuffix* CSuffixCollection::operator ^=(const QString& suffix)
{
      return this->find_or_fail ( suffix );
}
CSuffix* CSuffixCollection::find_or_fail(const QString& suffix)
{
    QMap<QString,CSuffix*>::iterator suffix_iter = m_suffix_map.find(suffix);
    //map_string_to_suffix_iter suffix_iter = m_SuffixMap.find(suffix);
    if (suffix_iter == m_suffix_map.end()){
        return NULL;
    } else{
        return suffix_iter.value();
    }

}
CSuffix* CSuffixCollection::find_or_add(const QString& suffix)
{    CSuffix* pSuffix;
    if (! m_suffix_map.contains(suffix)){
        CSuffix* pSuffix = new CSuffix(suffix);
        m_suffix_map.insert(suffix, pSuffix);
        m_suffix_list.append(pSuffix);
        //pSuffix->increment_count(1);
        return pSuffix;
    }
    pSuffix = m_suffix_map.value(suffix);
    //pSuffix->increment_count();
    return pSuffix;
}


CSuffix* CSuffixCollection::operator << (const QString& suffix)
{
    return this->find_or_add(suffix);
}
QList<CSuffix*>  CSuffixCollection::get_suffixes(){
    return m_suffix_list;
}

bool count_compare_suffixes(CSuffix* pSuff1, CSuffix* pSuff2){
    if (pSuff1->get_sig_count() > pSuff2->get_sig_count()) return true;
    return false;
}

void CSuffixCollection::sort_by_count()
{
    m_suffix_list.clear();
    QMapIterator<QString, CSuffix*> suffix_iter (m_suffix_map);
    while (suffix_iter.hasNext()){
        suffix_iter.next();
        m_suffix_list.append(suffix_iter.value());
    }
    std::sort(m_suffix_list.begin(), m_suffix_list.end(), count_compare_suffixes);

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
    m_prefix_list.clear();
}

CPrefix* CPrefixCollection::operator ^=(const QString& prefix)
{
      return this->find_or_fail ( prefix );
}
CPrefix* CPrefixCollection::find_or_fail(const QString& prefix)
{
    QMap<QString,CPrefix*>::iterator prefix_iter = m_PrefixMap.find(prefix);
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
      m_prefix_list.append(pPrefix);
      return pPrefix;
    }
    return prefix_iter.value();
}



CPrefix* CPrefixCollection::operator << (const QString& prefix)
{
    return this->find_or_add(prefix);
}
/*
void CPrefixCollection::get_set_of_prefixes(QSet<QString> *p_string_set){
    p_string_set->fromList(m_PrefixMap.uniqueKeys());
}
*/
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
        m_prefix_list.append(prefix_iter.value());
    }
    std::sort(m_prefix_list.begin(), m_prefix_list.end(), count_compare_prefixes);

}

