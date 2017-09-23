#ifndef CWORDCOLLECTION_H
#define CWORDCOLLECTION_H

#include <QList>
#include <iterator>
#include "generaldefinitions.h"
#include "StringSurrogate.h"
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
    CWord* operator<< ( CParse* );
    CWord* operator<< ( CStringSurrogate );
    CWord* operator<< ( QString );
    CWord* operator^= ( CParse* );
    CWord* operator^= ( CStringSurrogate );
    CWord* operator^= ( QString );

    CWord* find_or_add (QString);
    CWord* add (QString);
    CWord* find (QString); // returns false if the string is not in the word collection

    //CWord* GetAt( uint itemno) {return m_WordList.at(itemno);}
    int get_count() const { return m_WordMap.size(); }

    QMap<QString,CWord*>::iterator GetBegin()   { return m_WordMap.begin();  }
    QMap<QString,CWord*>::iterator GetEnd()     { return m_WordMap.end();    }
    QMap<QString,CWord*> *         GetList()    { return & m_WordMap;          }

    bool contains(QString string)  {return m_WordMap.contains(string);}
    void sort_word_list();
    QStringList* GetSortedStringArray() {return & m_SortedStringArray;}
    QMap<QString, CWord*>* GetMap() { return & m_WordMap; }


};

#endif // CWORDCOLLECTION_H


