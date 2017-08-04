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
    QList<CSuffix*> m_SuffixList;
    // map for suffixes?
public:
    //construction/destruction
    CSuffixCollection();
//    CWordCollection(CLexicon* Lex, QString MemberName = QString());
    ~CSuffixCollection();

// disable copy
private:
    CSuffixCollection(const CSuffixCollection& x);
    CSuffixCollection& operator=(const CSuffixCollection& x);
public:
    CSuffix* operator<< ( CParse* );
    CSuffix* operator<< ( CStringSurrogate );
    CSuffix* operator<< ( QString );
    CSuffix* operator^= ( CParse* );
    CSuffix* operator^= ( CStringSurrogate );
    CSuffix* operator^= ( QString );

    CSuffix* GetAt( uint );
    int GetLength() const { return m_SuffixList.length(); }

    QList<CSuffix*>::iterator GetBegin()   { return m_SuffixList.begin();  }
    QList<CSuffix*>::iterator GetEnd()     { return m_SuffixList.end();  }
};

#endif // CSUFFIXCOLLECTION_H
