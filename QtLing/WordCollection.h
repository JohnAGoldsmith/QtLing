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
    QList<CWord*> m_WordList;
    QMap<CStringSurrogate, CWord*> m_WordMap;
    int m_CorpusCount;
    QString m_MemberName;
    CWord** m_SortArray;
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
    CWord* operator<< ( CParse* );
    CWord* operator<< ( CStringSurrogate );
    CWord* operator<< ( QString );
    CWord* operator^= ( CParse* );
    CWord* operator^= ( CStringSurrogate );
    CWord* operator^= ( QString );

    CWord* GetAt( uint );
    int GetLength() const { return m_WordList.length(); }

    QList<CWord*>::iterator GetBegin()   { return m_WordList.begin();  }
    QList<CWord*>::iterator GetEnd()     { return m_WordList.end();    }
    QList<CWord*>           GetList()    { return m_WordList;          }

    QMap<CStringSurrogate, CWord*> GetMap() { return m_WordMap; }

//    QMapIterator<CStringSurrogate,CWord*> GetIter() { return QMapIterator<CStringSurrogate,CWord*>iter(m_WordMap); }

};

#endif // CWORDCOLLECTION_H


