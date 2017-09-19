#include "SuffixCollection.h"
#include <QDebug>
#include "Parse.h"
//#include "Suffix.h"

CSuffixCollection::CSuffixCollection()
{
    m_SuffixMap = QMap<QString, CSuffix*>();
}
CSuffixCollection::~CSuffixCollection() {}


CSuffix* CSuffixCollection::operator <<(QString suffix)
{ 
    CSuffix* pSuffix = new CSuffix(suffix);
    m_SuffixMap.insert(suffix, pSuffix);
    return pSuffix;
}

CSuffix* CSuffixCollection::find_or_add(QString suffix)
{

    QMap<QString,CSuffix*>::const_iterator suffix_iter = m_SuffixMap.find(suffix);

    if (suffix_iter == m_SuffixMap.end()){
      CSuffix* pSuffix = new CSuffix(suffix);
      m_SuffixMap.insert(suffix, pSuffix);
    }
    return suffix_iter.value();
}


CSuffix* CSuffixCollection::operator ^=(QString suffix)
{
    return this->find_or_add(suffix);
}
