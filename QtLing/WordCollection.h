#ifndef CWORDCOLLECTION_H
#define CWORDCOLLECTION_H

#include <QList>
#include <iterator>
#include "generaldefinitions.h"
#include <QMapIterator>
#include "Lexicon.h"
class CWord;



class CWordCollection
{
protected:

    QMap<QString, CWord*>   m_WordMap;
    int                     m_CorpusCount;
    QString                 m_MemberName;
    QStringList             m_SortedStringArray;
    QStringList             m_reverse_sort_list;
    CLexicon *              m_Lexicon;
    bool                    m_SortValidFlag;
    enum                    eSortStyle m_SortStyle;

    public:
    // construction/destruction.
    CWordCollection(CLexicon*);
    ~CWordCollection();

private:
    CWordCollection&        operator=(const CWordCollection& x);

public:
    // Many functions below used to take the type `QString` as arguments.
    // Changed them to `const QString&` for possibly greater efficiency.
    // Changed made by Hanson 7.30
    friend class CLexicon;
    CWord*                  operator<< (const QString&);
    CWord*                  operator^= (const QString&);

    CWord*                  add (const QString&);
    bool                    contains(const QString& string)  {return m_WordMap.contains(string);}
    CWord*                  find_or_add (const QString&);
    CWord*                  find_or_fail(const QString& word_t);
    int                     get_count() const       { return m_WordMap.size(); }
    CWord*                  get_word (const QString&);     // returns false if the string is not in the word collection
    QString                 get_word_string (int n) {return m_SortedStringArray[n];}
    QStringList*            get_reverse_sort_list() {return & m_reverse_sort_list;}
    QStringList*            GetSortedStringArray() {return & m_SortedStringArray;}
    QMap<QString, CWord*>*  GetMap() { return & m_WordMap; }
    QMap<QString, CWord*>*  get_map() { return & m_WordMap; }
    void                    sort_word_list();

    void                    assign_json_id();
    void                    write_json(QJsonObject& ref_json, eJsonType json_type = INDEXED);

    QMapIterator<QString,CWord*> *  get_iterator();
    QMap<word_t, CWord*>::iterator  begin()                    { return m_WordMap.begin();}
    QMap<word_t, CWord*>::iterator  end()                      { return m_WordMap.end();}
};

#endif // CWORDCOLLECTION_H


