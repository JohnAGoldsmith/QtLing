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

    QMap<QString  , CStem*> m_StringToStemMap;
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
    CStem* find_stem(QString);  // same as ˆ=

    CStem* GetAtKey( QString);
    int GetSize() const { return m_StringToStemMap.size(); }
    void add_stem_pointer (CStem* pStem);
    
    QMap<QString,CStem*>::iterator GetBegin()   { return m_StringToStemMap.begin();  }
    QMap<QString,CStem*>::iterator GetEnd()     { return m_StringToStemMap.end();  }
    
    
};

#endif // CSTEMCOLLECTION_H
