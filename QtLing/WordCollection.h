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
    //QList<CWord*> m_WordList;
    QMap<QString, CWord*>   m_WordMap;
    int                     m_CorpusCount;
    QString                 m_MemberName;
    QStringList             m_SortedStringArray;
    QStringList             m_reverse_sort_list;
    CLexicon *               m_Lexicon;
    bool                    m_SortValidFlag;
    enum                    eSortStyle m_SortStyle;

    public:
    // construction/destruction.
    CWordCollection(CLexicon*);
    ~CWordCollection();

private:
    CWordCollection& operator=(const CWordCollection& x);

public:
    friend class CLexicon;
    CWord* operator<< ( QString );
    CWord* operator^= ( QString );

    CWord* find_or_add (QString);
    CWord* find_or_null (QString);
    CWord* add (QString);
    CWord* get_word (QString); // returns false if the string is not in the word collection
    QString get_word_string (int n) {return m_SortedStringArray[n];}
    //CWord* GetAt( uint itemno) {return m_WordList.at(itemno);}
    int get_count() const { return m_WordMap.size(); }

    QMapIterator<QString,CWord*> *  get_iterator();

    CWord*                  find_or_fail(QString word_t);
    bool                    contains(QString string)  {return m_WordMap.contains(string);}
    void                    sort_word_list();
    QStringList*              get_reverse_sort_list() {return & m_reverse_sort_list;}
    QStringList*            GetSortedStringArray() {return & m_SortedStringArray;}
    QMap<QString, CWord*>*  GetMap() { return & m_WordMap; }
    QMap<QString, CWord*>*  get_map() { return & m_WordMap; }
    QMap<word_t, CWord*>::iterator  begin()                    { return m_WordMap.begin();}
    QMap<word_t, CWord*>::iterator  end()                      { return m_WordMap.end();}
};

#endif // CWORDCOLLECTION_H


