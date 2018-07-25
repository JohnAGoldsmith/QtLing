#ifndef CSUFFIXCOLLECTION_H
#define CSUFFIXCOLLECTION_H

#include <QList>
#include <iterator>
#include "generaldefinitions.h"
#include "Suffix.h"
#include "Typedefs.h"

class CLexicon;


class CSuffixCollection
{
protected:
    QMap<QString, CSuffix*>  m_SuffixMap;
    CSuffix_ptr_list         m_SortedList;
    CLexicon*              m_Lexicon;

public:
    //construction/destruction
    CSuffixCollection(CLexicon*);
    ~CSuffixCollection();

// disable copy
private:
    CSuffixCollection(const CSuffixCollection& x);
    CSuffixCollection& operator=(const CSuffixCollection& x);
public:

    CSuffix*                operator<< (const QString&);
    CSuffix*                operator^= (const QString&);
    CSuffix*                find_or_add(const QString&); //same as << (find if it is there, else add)
    CSuffix*                find_or_fail(const QString&); //same as ˆ= (find if it is there, else void)
    CSuffix*                GetAt( uint );
    int                     get_count() const               { return m_SuffixMap.size(); }
    void                    get_set_of_suffixes (QSet<QString> * p_string_set);
    QMap<QString,CSuffix*>* GetMap()                        { return & m_SuffixMap;         }
    QMap<QString,CSuffix*>* get_map()                       { return & m_SuffixMap;         }
    void                    get_suffixes(QList<QString>*);
    bool                    contains(suffix_t this_suffix)  {return m_SuffixMap.contains(this_suffix);}
    void                    sort_by_count();
    CSuffix_ptr_list*       get_sorted_list()               {return & m_SortedList;}
    void                    clear();
    void                    remove_suffix(CSuffix* p_suffix);
};

class CPrefixCollection
{
protected:
    QMap<QString, CPrefix*> m_PrefixMap;
    CLexicon*               m_Lexicon;
    CPrefix_ptr_list         m_SortedList;
public:
    //construction/destruction
    CPrefixCollection(CLexicon* );
    ~CPrefixCollection();

// disable copy
private:
    CPrefixCollection(const CPrefixCollection& x);
    CPrefixCollection& operator=(const CPrefixCollection& x);
public:

    CPrefix*                operator<< (const QString&);
    CPrefix*                operator^= (const QString&);
    CPrefix*                find_or_add        (const QString&); //same as << (find if it is there, else add)
    CPrefix*                find_or_fail        (const QString&); //same as ˆ= (find if it is there, else void)
    CPrefix*                GetAt( uint );
    int                     get_count() const { return m_PrefixMap.size(); }
    void                    get_set_of_prefixes (QSet<QString> * p_string_set);
    QMap<QString,CPrefix*>* GetMap()               { return & m_PrefixMap;         }
    QMap<QString,CPrefix*>* get_map()              { return & m_PrefixMap;         }
    void                    get_prefixes(QList<QString>*);
    bool                    contains(prefix_t this_prefix) {return m_PrefixMap.contains(this_prefix);}
    void                    sort_by_count();
    CPrefix_ptr_list*       get_sorted_list()               {return & m_SortedList;}
    void                    clear();
    void                    remove_prefix(CPrefix* p_prefix);

};

#endif // CSUFFIXCOLLECTION_H
