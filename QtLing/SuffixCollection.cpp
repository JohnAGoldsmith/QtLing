#include "SuffixCollection.h"
#include <QDebug>
#include "Parse.h"
//#include "Suffix.h"

CSuffixCollection::CSuffixCollection()
{
    m_SuffixList = QList<CSuffix*>();
}
CSuffixCollection::~CSuffixCollection() {}

CSuffix* CSuffixCollection::operator<<( CParse* pParse )
{
    CStringSurrogate  cssKey;
    cssKey = pParse->GetKey();

    CSuffix* Suffix = new CSuffix(cssKey);
    m_SuffixList << Suffix;
    m_SuffixList[cssKey] = Suffix;
    return Suffix;
}
CSuffix* CSuffixCollection::operator <<(CStringSurrogate SS)
{
    CSuffix* Suffix = new CSuffix(SS);
    m_SuffixList << Suffix;
    return Suffix;
}
CSuffix* CSuffixCollection::operator <<(QString szSuffix)
{
    const QChar* key = szSuffix.constData();
    CStringSurrogate cssKey = CStringSurrogate(key, szSuffix.length());
    CSuffix* Suffix = new CSuffix(cssKey);
    m_SuffixList << Suffix;
    return Suffix;
}
CSuffix* CSuffixCollection::operator ^=(CParse* string)
{
    CStringSurrogate cssKey = string->GetKey();

    CSuffix* pSuffix = new CSuffix(cssKey);
    int length = m_SuffixList.length();
    for (int i = 0 ; i < length; i++) {
        if (pSuffix->GetSuffix() == m_SuffixList.at(i)->GetSuffix()) {
            return pSuffix;
        }
    }
    return 0;
}
CSuffix* CSuffixCollection::operator ^=(CStringSurrogate string)
{
    CSuffix* pSuffix = new CSuffix(string);
    int length = m_SuffixList.length();
    for (int i = 0 ; i < length; i++) {
        if (pSuffix->GetSuffix() == m_SuffixList.at(i)->GetSuffix()) {
            return pSuffix;
        }
    }
    return 0;
}
CSuffix* CSuffixCollection::operator ^=(QString szSuffix)
{
    CStringSurrogate cssKey = CStringSurrogate(szSuffix, szSuffix.length());
    CSuffix* pSuffix = new CSuffix(cssKey);

    int length = m_SuffixList.length();
    for (int i = 0 ; i < length; i++) {
        if (pSuffix->GetSuffix() == m_SuffixList.at(i)->GetSuffix()) {
            return pSuffix;
        }
    }
    return 0;
}
CSuffix* CSuffixCollection::GetAt( uint n )
{
    return m_SuffixList.at(n);
}
