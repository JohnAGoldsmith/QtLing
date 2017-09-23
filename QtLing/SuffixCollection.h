#ifndef CSUFFIXCOLLECTION_H
#define CSUFFIXCOLLECTION_H

#include <QList>
#include <iterator>
#include "generaldefinitions.h"
#include "StringSurrogate.h"
#include "Suffix.h"

//class CSuffix;
class CLexicon;
class CParse;

class CSuffixCollection
{
protected:
    QMap<QString, CSuffix*> m_SuffixMap;

public:
    //construction/destruction
    CSuffixCollection();
    ~CSuffixCollection();

// disable copy
private:
    CSuffixCollection(const CSuffixCollection& x);
    CSuffixCollection& operator=(const CSuffixCollection& x);
public:

    CSuffix* operator<< ( QString );
    CSuffix* operator^= ( QString );
    CSuffix* find_or_add        ( QString ); //same as ˆ= (find if it is there, else add)
    CSuffix* GetAt( uint );
    int get_count() const { return m_SuffixMap.size(); }

    QMap<QString,CSuffix*>::iterator GetBegin()   { return m_SuffixMap.begin(); }
    QMap<QString,CSuffix*>::iterator GetEnd()     { return m_SuffixMap.end();   }
    QMap<QString,CSuffix*> GetMap()              { return m_SuffixMap;         }
};

#endif // CSUFFIXCOLLECTION_H
