#include "StemCollection.h"
#include <QDebug>
#include "Parse.h"
//#include "Stem.h"

CStemCollection::CStemCollection()
{
    m_StemList              = QList<CStem>();
    m_CorpusCount			= 0;
    m_MemberName			= QString::null;
    m_SortArray				= NULL;
    m_SortValidFlag			= 0;
    m_SortStyle				= KEY;
}

//CStemCollection::CStemCollection(CLexicon* Lex, QString MemberName)
//{
//    m_StemList              = QList<CStem>();
//    m_CorpusCount			= 0;
//    m_MemberName			= QString::null;
//    m_SortArray				= NULL;
//    m_SortValidFlag			= 0;
//    m_SortStyle				= KEY;
//}
CStemCollection::~CStemCollection()
{
    if ( m_SortArray )         { delete [] m_SortArray; m_SortArray = NULL;  }
}
CStem* CStemCollection::operator<<( CParse* pParse )
{
    CStringSurrogate  cssKey;
    cssKey = pParse->GetKey();

    CStem* Stem = new CStem(cssKey);
//    Stem->IncrementStemCount();
    m_StemList << Stem;
    m_StemMap[cssKey] = Stem;
    return Stem;
}
CStem* CStemCollection::operator <<(CStringSurrogate SS)
{
    CStem* Stem = new CStem(SS);
//    Stem->IncrementStemCount();
    m_StemList << Stem;
    m_StemMap[SS] = Stem;
    return Stem;
}
CStem* CStemCollection::operator <<(QString szStem)
{
    const QChar* key = szStem.constData();
    CStringSurrogate cssKey = CStringSurrogate(key, szStem.length());
    CStem* Stem = new CStem(cssKey);
    m_StemList << Stem;
    m_StemMap[cssKey] = Stem;
    return Stem;
}
CStem* CStemCollection::operator ^=(CParse* string)
{
    CStringSurrogate cssKey = string->GetKey();

    CStem* pStem = new CStem(cssKey);
    int length = m_StemList.length();
    for (int i = 0 ; i < length; i++) {
        if (pStem->GetStem() == m_StemList.at(i)->GetStem()) {
//            pStem->IncrementStemCount();
            return pStem;
        }
    }
    return 0;
}
CStem* CStemCollection::operator ^=(CStringSurrogate string)
{
    CStem* pStem = new CStem(string);
    int length = m_StemList.length();
    for (int i = 0 ; i < length; i++) {
        if (pStem->GetStem() == m_StemList.at(i)->GetStem()) {
//            pStem->IncrementStemCount();
            return pStem;
        }
    }
    return 0;
}
CStem* CStemCollection::operator ^=(QString szStem)
{
    CStringSurrogate cssKey = CStringSurrogate(szStem, szStem.length());
    CStem* pStem = new CStem(cssKey);

    int length = m_StemList.length();
    for (int i = 0 ; i < length; i++) {
        if (pStem->GetStem() == m_StemList.at(i)->GetStem()) {
//            pStem->IncrementStemCount();
            return pStem;
        }
    }
    return 0;
}
CStem* CStemCollection::GetAt( uint n )
{
    return m_StemList.at(n);
}
