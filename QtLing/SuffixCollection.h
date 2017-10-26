#ifndef CSUFFIXCOLLECTION_H
#define CSUFFIXCOLLECTION_H

#include <QList>
#include <iterator>
#include "generaldefinitions.h"
#include "Suffix.h"


class CLexicon;


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

    CSuffix*                operator<< ( QString );
    CSuffix*                operator^= ( QString );
    CSuffix*                find_or_add        ( QString ); //same as << (find if it is there, else add)
    CSuffix*                find_or_fail        ( QString ); //same as ˆ= (find if it is there, else void)
    CSuffix*                GetAt( uint );
    int                     get_count() const { return m_SuffixMap.size(); }
    void                    get_set_of_suffixes (QSet<QString> * p_string_set);
    QMap<QString,CSuffix*>  GetMap()              { return m_SuffixMap;         }
    void                    get_suffixes(QList<QString>*);
    bool                    contains(suffix_t this_suffix) {return m_SuffixMap.contains(this_suffix);}
};

#endif // CSUFFIXCOLLECTION_H
