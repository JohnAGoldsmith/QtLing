#ifndef CWORD_H
#define CWORD_H

#include "Parse.h"
#include "StringSurrogate.h"
#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
#include "generaldefinitions.h"
#include <QPair>
#include <tuple>
#include <iostream>

#include "Stem.h"
#include "Suffix.h"
#include "Signature.h"

//typedef QPair<CStem,QPair<CSuffix, CSignature>> WordParse;
struct Parse {
    CStem* p_stem;
    CSuffix* p_suffix;
    CSignature* p_signature;
    QList<CSignature*>  m_Signatures;
};

class CWord : public CParse
{
protected:
    QString m_Word;
    int m_WordCount;
    QList<Parse> m_Parses;
    QList<CSignature*>  m_Signatures;
public:
    CWord(CStringSurrogate ssWord);
    CWord(CWord&);

public:
    //Accessors
    int                     GetWordCount() const { return m_WordCount; }
    void                    SetWordCount(int count) { m_WordCount = count;}
    QString                 GetWord()      const { return m_Word; }

    QList<Parse>            GetParses()    const { return m_Parses; }
    QList<CSignature*> *    GetSignatures ()   {return &m_Signatures;}
    void                    AddParse(CStem* stem, CSuffix* suffix, CSignature* signature);
    void                    add_signature(CSignature*);
    void                    IncrementWordCount(int n = 1);

};

#endif // CWORD_H
