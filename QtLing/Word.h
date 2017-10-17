#ifndef CWORD_H
#define CWORD_H

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


typedef  QPair<CStem*,CSignature*>* ptr_to_stem_sig_pair;
struct Parse {
    CStem* p_stem;
    CSuffix* p_suffix;
    CSignature* p_signature;
   // QList<QPair<CStem*,CSignature*>>  m_Signatures;
};

class CWord
{
protected:
    QString                             m_Word;
    int                                 m_WordCount;
    QList<Parse>                        m_Parses;
    QList<ptr_to_stem_sig_pair>         m_Signatures;
public:
    CWord(QString  Word);
    CWord(CWord&);

public:
    //Accessors
    int                     GetWordCount() const { return m_WordCount; }
    void                    SetWordCount(int count) { m_WordCount = count;}
    QString                 GetWord()      const { return m_Word; }
    QString                 get_key() {return m_Word;}

    QList<Parse>                            GetParses()    const { return m_Parses; }
    QList<QPair<CStem*,CSignature*>*> *      GetSignatures ()   {return &m_Signatures;}
    void                                    AddParse(CStem* stem, CSuffix* suffix, CSignature* signature);
    void                                    add_stem_and_signature(CStem*, CSignature*);
    void                                    IncrementWordCount(int n = 1);

};

#endif // CWORD_H
