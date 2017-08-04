#ifndef CSTEMCOLLECTION_H
#define CSTEMCOLLECTION_H

#include <QList>
#include <iterator>
#include "generaldefinitions.h"
#include "StringSurrogate.h"
#include "Stem.h"

//class CStem;
class CLexicon;
class CParse;

class CStemCollection
{
protected:
    QList<CStem*> m_StemList;
    QMap<CStringSurrogate, CStem*> m_StemMap;
    int m_CorpusCount;
    QString m_MemberName;
    CStem** m_SortArray;
    bool m_SortValidFlag;
    enum eSortStyle m_SortStyle;

public:
    CStemCollection();
//    CStemCollection(CLexicon* Lex, QString MemberName = QString());
    ~CStemCollection();

// disable copy
private:
    CStemCollection(const CStemCollection& x);
    CStemCollection& operator=(const CStemCollection& x);

public:
    CStem* operator<< ( CParse* );
    CStem* operator<< ( CStringSurrogate );
    CStem* operator<< ( QString );
    CStem* operator^= ( CParse* );
    CStem* operator^= ( CStringSurrogate );
    CStem* operator^= ( QString );

    CStem* GetAt( uint );
    int GetLength() const { return m_StemList.length(); }

    QList<CStem*> GetList()              { return m_StemList; }
    QList<CStem*>::iterator GetBegin()   { return m_StemList.begin();  }
    QList<CStem*>::iterator GetEnd()     { return m_StemList.end();  }
};

#endif // CSTEMCOLLECTION_H
