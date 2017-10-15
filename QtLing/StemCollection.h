#ifndef CSTEMCOLLECTION_H
#define CSTEMCOLLECTION_H

#include <QList>
#include <iterator>
#include "generaldefinitions.h"
//#include "StringSurrogate.h"
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
    QList<CStem*>                 m_SortList;
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
//    CStem* operator<< ( CStringSurrogate );
    CStem* operator<< ( QString );
    CStem* operator^= ( CParse* );
//    CStem* operator^= ( CStringSurrogate );
    CStem* operator^= ( QString );
    CStem* find_or_add (QString);  // same as ˆ=
    CStem* find (QString);
    CStem* add (QString);

    CStem* GetAtKey( QString);
    int get_count() const { return m_StringToStemMap.size(); }
    void add_stem_pointer (CStem* pStem);

    
    QMapIterator<QString,CStem*> * get_map_iterator();
    QListIterator<CStem*> * get_sorted_list_iterator();
    
};

#endif // CSTEMCOLLECTION_H
