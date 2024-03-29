#ifndef CSUFFIXCOLLECTION_H
#define CSUFFIXCOLLECTION_H

#include <QList>
#include <iterator>
#include <QJsonObject>
#include "generaldefinitions.h"
#include "Suffix.h"
#include "Typedefs.h"

class CLexicon;
class CJsonInfo;


class CSuffixCollection
{
protected:
    QMap<QString, CSuffix*>  m_suffix_map;   // json tag: "suffixes"
    CSuffix_ptr_list         m_suffix_list;  // json tag: "sortedSuffixes"
    CLexicon*                m_Lexicon;

public:
    //construction/destruction
    CSuffixCollection(CLexicon*);
    ~CSuffixCollection();

// disable copy
private:
    CSuffixCollection(const CSuffixCollection& x);
    CSuffixCollection& operator=(const CSuffixCollection& x);
public:
    // Many functions below used to take the type `QString` as arguments.
    // Changed them to `const QString&` for possibly greater efficiency.
    // Changed made by Hanson 7.30

    CSuffix*                operator<< (const QString&);
    CSuffix*                operator^= (const QString&);
    void                    clear();
    CSuffix*                find_or_add(const QString&); //same as << (find if it is there, else add)
    CSuffix*                find_or_fail(const QString&); //same as ˆ= (find if it is there, else void)
    CSuffix*                GetAt( uint );
    int                     get_count() const               { return m_suffix_map.size(); }
    //void                    get_set_of_suffixes (QSet<QString> * p_string_set);
    QMap<QString,CSuffix*>* GetMap()                        { return & m_suffix_map; }
    QMap<QString,CSuffix*>* get_map()                       { return & m_suffix_map; }
    QList<CSuffix*>         get_suffixes( );
    QList<CSuffix*>    *    get_suffix_list()               {return & m_suffix_list;}
    bool                    contains(const suffix_t& this_suffix)  { return m_suffix_map.contains(this_suffix); }
    void                    sort_by_count();
    CSuffix_ptr_list*       get_sorted_list()               { return & m_suffix_list; }


    void                    assign_json_id();
    void                    write_json(QJsonObject& ref_json, eJsonType json_type = INDEXED);
    void                    read_json_1(const QJsonObject& ref_json, CJsonInfo& ref_pointers);
};

class CPrefixCollection
{
protected:
    QMap<QString, CPrefix*> m_PrefixMap;
    CLexicon*               m_Lexicon;
    CPrefix_ptr_list         m_prefix_list;
public:
    //construction/destruction
    CPrefixCollection(CLexicon* );
    ~CPrefixCollection();

// disable copy
private:
    CPrefixCollection(const CPrefixCollection& x);
    CPrefixCollection& operator=(const CPrefixCollection& x);
public:

    //CPrefix*                operator<< (const QString&);
    //CPrefix*                operator^= (const QString&);
    CPrefix_ptr_list *       get_prefix_list() {return &m_prefix_list;}
    int                     get_count() const               { return m_PrefixMap.size(); }
    CPrefix*                find_or_add        (const QString&); //same as << (find if it is there, else add)
    CPrefix*                find_or_fail        (const QString&); //same as ˆ= (find if it is there, else void)
    CPrefix*                GetAt( uint );
    //void                    get_set_of_prefixes (QSet<QString> * p_string_set);
    QMap<QString,CPrefix*>* GetMap()               { return & m_PrefixMap;         }
    QMap<QString,CPrefix*>* get_map()              { return & m_PrefixMap;         }
    void                    get_prefixes(QList<QString>*);
    bool                    contains(const prefix_t& this_prefix) {return m_PrefixMap.contains(this_prefix);}
    void                    sort_by_count();
    CPrefix_ptr_list*       get_sorted_list()               {return & m_prefix_list;}
    void                    clear();

    void                    assign_json_id();
    void                    write_json(QJsonObject& ref_json, eJsonType json_type = INDEXED);
    void                    read_json_1(const QJsonObject &ref_json, CJsonInfo& ref_pointers);

};

#endif // CSUFFIXCOLLECTION_H
