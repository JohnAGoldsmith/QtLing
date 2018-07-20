#include "SuffixCollection.h"
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

bool count_compare_suffixes(CSuffix* pSuff1, CSuffix* pSuff2){
    if (pSuff1->get_count() > pSuff2->get_count()) return true;
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

void CSuffixCollection::assign_json_id()
{
    QMap<QString, CSuffix*>::ConstIterator map_iter;
    int curr_json_id = 0;
    for (map_iter = m_SuffixMap.constBegin();
         map_iter != m_SuffixMap.constEnd();
         map_iter++) {
        CSuffix* p_suffix = map_iter.value();
        p_suffix->set_json_id(curr_json_id);
    }
}

void CSuffixCollection::write_json(QJsonObject &ref_json)
{
    ref_json["count"] = m_SuffixMap.size();

    assign_json_id();
    QJsonArray arr_suffixes;
    QMap<QString, CSuffix*>::ConstIterator suffix_iter;
    for (suffix_iter = m_SuffixMap.constBegin();
         suffix_iter != m_SuffixMap.constEnd();
         suffix_iter++) {
        CSuffix* p_suffix = suffix_iter.value();
        QJsonObject json_suffix;
        p_suffix->write_json(json_suffix);
        arr_suffixes.append(json_suffix);
    }
    QJsonArray arr_sorted_suffixes;
    QList<CSuffix*>::ConstIterator sorted_iter;
    for (sorted_iter = m_SortedList.constBegin();
         sorted_iter != m_SortedList.constEnd();
         sorted_iter++) {
        CSuffix* p_suffix = *sorted_iter;
        arr_sorted_suffixes.append(p_suffix->get_json_id());
    }

    ref_json["suffixes"] = arr_suffixes;
    ref_json["sortedSuffixes"] = arr_sorted_suffixes;
}

void CSuffixCollection::read_json(const QJsonObject &ref_json)
{
    m_SuffixMap.clear();
    m_SortedList.clear();
    m_SortedList.reserve(ref_json["count"].toInt());
    QJsonArray::ConstIterator iter;
    const QJsonArray& ref_suffixes = ref_json["suffixes"].toArray();
    const QJsonArray& ref_sorted_suffixes = ref_json["sortedSuffixes"].toArray();
    for (iter = ref_suffixes.constBegin();
         iter != ref_suffixes.constEnd();
         iter++) {
        const QJsonObject& obj_suffix = iter->toObject();
        const QString& str_suffix = obj_suffix["suffix"].toString();
        CSuffix* p_suffix = new CSuffix(str_suffix);
        p_suffix->read_json(obj_suffix);
        m_SuffixMap.insert(str_suffix, p_suffix);
    }
    sort_by_count();
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

CPrefix* CPrefixCollection::operator ^=(QString prefix)
{
      return this->find_or_fail ( prefix );
}
CPrefix* CPrefixCollection::find_or_fail(QString prefix)
{
    QMap<QString,CPrefix*>::iterator prefix_iter = m_PrefixMap.find(prefix);
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
bool count_compare_prefixes(CPrefix* pPrefix1, CPrefix* pPrefix2){
    if (pPrefix1->get_count() > pPrefix2->get_count()) return true;
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

void CPrefixCollection::assign_json_id()
{
    QMap<QString, CPrefix*>::ConstIterator map_iter;
    int curr_json_id = 0;
    for (map_iter = m_PrefixMap.constBegin();
         map_iter != m_PrefixMap.constEnd();
         map_iter++) {
        CPrefix* p_prefix = map_iter.value();
        p_prefix->set_json_id(curr_json_id);
    }
}

void CPrefixCollection::write_json(QJsonObject &ref_json)
{
    ref_json["count"] = m_PrefixMap.size();

    assign_json_id();
    QJsonArray arr_prefixes;
    QMap<QString, CPrefix*>::ConstIterator prefix_iter;
    for (prefix_iter = m_PrefixMap.constBegin();
         prefix_iter != m_PrefixMap.constEnd();
         prefix_iter++) {
        CPrefix* p_prefix = prefix_iter.value();
        QJsonObject json_prefix;
        p_prefix->write_json(json_prefix);
        arr_prefixes.append(json_prefix);
    }
    QJsonArray arr_sorted_prefixes;
    QList<CPrefix*>::ConstIterator sorted_iter;
    for (sorted_iter = m_SortedList.constBegin();
         sorted_iter != m_SortedList.constEnd();
         sorted_iter++) {
        CPrefix* p_prefix = *sorted_iter;
        arr_sorted_prefixes.append(p_prefix->get_json_id());
    }

    ref_json["prefixes"] = arr_prefixes;
    ref_json["sortedPrefixes"] = arr_sorted_prefixes;
}

void CPrefixCollection::read_json(const QJsonObject &ref_json)
{
    m_PrefixMap.clear();
    m_SortedList.clear();
    m_SortedList.reserve(ref_json["count"].toInt());
    QJsonArray::ConstIterator iter;
    const QJsonArray& ref_prefixes = ref_json["prefixes"].toArray();
    const QJsonArray& ref_sorted_prefixes = ref_json["sortedPrefixes"].toArray();
    for (iter = ref_prefixes.constBegin();
         iter != ref_prefixes.constEnd();
         iter++) {
        const QJsonObject& obj_prefix = iter->toObject();
        const QString& str_prefix = obj_prefix["prefix"].toString();
        CPrefix* p_prefix = new CPrefix(str_prefix);
        p_prefix->read_json(obj_prefix);
        m_PrefixMap.insert(str_prefix, p_prefix);
    }
    sort_by_count();
}
