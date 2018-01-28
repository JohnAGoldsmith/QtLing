#ifndef CWORD_H
#define CWORD_H

#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
//#include "generaldefinitions.h"
#include <QPair>
#include <tuple>
#include <iostream>

#include "Stem.h"
#include "Suffix.h"
#include "Signature.h"

class LowerTableView;

typedef  QPair<CStem*,CSignature*>* ptr_to_stem_sig_pair;

struct Parse_triple {
    QString         p_stem;
    QString         p_suffix;
    CSignature*     p_signature;

    Parse_triple(QString stem, QString suffix, CSignature*  pSig){p_stem = stem; p_suffix = suffix; p_signature=pSig;}
};


class CWord
{ friend LowerTableView;
protected:
    QString                             m_Word;
    int                                 m_WordCount;
    QList<Parse_triple*>                        m_Parse_triples;
    QList<ptr_to_stem_sig_pair>         m_Signatures;
    QList<QString>                      m_Autobiography;
public:
    CWord(QString  Word);
    CWord(CWord&);
    ~CWord();
public:
    //Accessors
    void                                add_parse(CStem* stem, CSuffix* suffix, CSignature* signature);
//    void                                add_stem_and_signature(CStem*, CSignature*);
    void                                add_parse_triple(QString stem, QString affix, CSignature*);
    void                                add_to_autobiography(QString line);
    void                                clear_signatures() { m_Signatures.clear();}
    void                                clear_parse_triples() {m_Parse_triples.clear();}
    QList<QString>*                     get_autobiography() {return & m_Autobiography;}
    QString                             get_key() {return m_Word;}
    int                                 get_word_count() const  { return m_WordCount; }
    const QList<Parse_triple*>*         get_parse_triples()     const  { return & m_Parse_triples; }
    QList<QPair<CStem*,CSignature*>*> * get_signatures ()   {return &m_Signatures;}
    QString                             GetWord()      const { return m_Word; }
    void                                IncrementWordCount(int n = 1);
    void                                SetWordCount(int count) { m_WordCount = count;}

};

#endif // CWORD_H
