#ifndef CWORDCOLLECTION_H
#define CWORDCOLLECTION_H

#include <QList>
#include <iterator>
#include "generaldefinitions.h"
#include <QMapIterator>

class CWord;
class CLexicon;
class CParse;

class CWordCollection
{
protected:
    //QList<CWord*> m_WordList;
    QMap<QString, CWord*> m_WordMap;
    int m_CorpusCount;
    QString m_MemberName;
    QStringList m_SortedStringArray;
    bool m_SortValidFlag;
    enum eSortStyle m_SortStyle;

    public:
    // construction/destruction.
    CWordCollection();
//    CWordCollection(CLexicon* Lex, QString MemberName = QString());
    ~CWordCollection();

// disable copy
private:
    CWordCollection(const CWordCollection& x);
    CWordCollection& operator=(const CWordCollection& x);

public:
    friend class CLexicon;
    CWord* operator<< ( QString );
    CWord* operator^= ( QString );

    CWord* find_or_add (QString);
    CWord* find_or_null (QString);
    CWord* add (QString);
    CWord* get_word (QString); // returns false if the string is not in the word collection

    //CWord* GetAt( uint itemno) {return m_WordList.at(itemno);}
    int get_count() const { return m_WordMap.size(); }

    QMapIterator<QString,CWord*> *  get_iterator();

    CWord*                  find_or_fail(QString word_t);
    bool                    contains(QString string)  {return m_WordMap.contains(string);}
    void                    sort_word_list();
    QStringList*            GetSortedStringArray() {return & m_SortedStringArray;}
    QMap<QString, CWord*>*  GetMap() { return & m_WordMap; }


};

#endif // CWORDCOLLECTION_H


