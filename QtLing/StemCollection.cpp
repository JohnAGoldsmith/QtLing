#include "StemCollection.h"
#include <QDebug>
#include "Parse.h"
//#include "Stem.h"

CStemCollection::CStemCollection()
{
   // m_StemList              = QList<CStem*>();
    m_CorpusCount			= 0;
    m_MemberName			= QString::null;
    m_SortArray				= NULL;
    m_SortValidFlag			= 0;
    m_SortStyle				= KEY;
}


CStemCollection::~CStemCollection()
{
    if ( m_SortArray )         { delete [] m_SortArray; m_SortArray = NULL;  }
}

CStem* CStemCollection::operator <<(QString szStem)
{
    CStem* pStem = new CStem(szStem);
    m_StringToStemMap[szStem] = pStem;
    return pStem;
}

CStem* CStemCollection::operator ^=(QString szStem)
{ if (m_StringToStemMap.contains(szStem)){
    return m_StringToStemMap.value(szStem);
}
    CStem* pStem = new CStem(szStem);
    return pStem;
}
//CStem* CStemCollection::GetAt( uint n )
//{
//    return m_StemList.at(n);
//}

CStem* CStemCollection::GetAtKey( QString stem)
{
    return m_StringToStemMap[stem];
}

CStem* CStemCollection::find_stem(QString stem)
{
    if (m_StringToStemMap.contains(stem)){
        return m_StringToStemMap.value(stem);
    } else{
        CStem* pStem = new CStem(stem);
        return pStem;
    }
}

