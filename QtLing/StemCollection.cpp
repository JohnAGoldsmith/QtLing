#include "StemCollection.h"
#include <QDebug>
#include "Parse.h"
//#include "Stem.h"

CStemCollection::CStemCollection()
{
   // m_StemList              = QList<CStem*>();
    m_CorpusCount			= 0;
    m_MemberName			= QString::null;
    //m_SortArray				= NULL;
    m_SortValidFlag			= 0;
    m_SortStyle				= KEY;
}


CStemCollection::~CStemCollection()
{
    //if ( m_SortArray )         { delete [] m_SortArray; m_SortArray = NULL;  }
}

CStem* CStemCollection::add(QString stem)
{
    CStem* pStem = new CStem(stem);
    m_StringToStemMap[stem] = pStem;
    return pStem;
}
CStem* CStemCollection::operator <<(QString stem)
{
    return this->add(stem);
}

CStem* CStemCollection::operator ^=(QString stem)
{ return this->find_or_add(stem);
}


CStem* CStemCollection::GetAtKey( QString stem)
{
    return m_StringToStemMap[stem];
}

CStem* CStemCollection::find_or_add(QString stem)
{
    if (m_StringToStemMap.contains(stem)){
        return m_StringToStemMap.value(stem);
    } else{
        CStem* pStem = new CStem(stem);
        m_StringToStemMap[stem] = pStem;
        return pStem;
    }
}

CStem* CStemCollection::find(QString stem)
{

}
