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
    QString         m_stem;
    QString         m_suffix;
//    CSignature*     p_signature; // we will get rid of this, since it can be a hanging pointer when the lexicon changes.
    QString         m_sig_string;

    Parse_triple(QString stem, QString suffix, QString sig_string)
        {m_stem = stem; m_suffix = suffix;  m_sig_string =sig_string;}
};


class CWord
{
    friend class LowerTableView;
protected:
    QString                             m_Word;
    int                                 m_WordCount;
//    QList<Parse_triple*>                m_Parse_triples;
    // Instead of keeping a list of parse_triples, we keep a map where the key is the stem, and the value is the parse_triple
    QMap<stem_t, Parse_triple*>         m_Parse_triple_map;
    QList<ptr_to_stem_sig_pair>         m_Signatures;
    QList<QString>                      m_Autobiography;
    int                                 m_json_id;
public:
    CWord(QString  Word);
    CWord(CWord&);
    ~CWord();
public:
    //Accessors
    void                                add_parse(CStem* stem, CSuffix* suffix, CSignature* signature);
//    void                                add_parse_triple(QString stem, QString affix, CSignature*);
    void                                add_parse_triple(QString stem, QString affix, sig_string this_sig_string);
    void                                add_to_autobiography(QString line);
    void                                clear_signatures() { m_Signatures.clear();}
    void                                clear_parse_triple_map();
    QList<QString>*                     get_autobiography() {return & m_Autobiography;}
    QString                             get_key() {return m_Word;}
    int                                 get_word_count() const  { return m_WordCount; }
    QMap<stem_t,Parse_triple*>*         get_parse_triple_map()     { return & m_Parse_triple_map; }
    QList<QPair<CStem*,CSignature*>*> * get_signatures ()   {return &m_Signatures;}
    QString                             get_word()      const { return m_Word; }
    void                                IncrementWordCount(int n = 1);
    void                                SetWordCount(int count) { m_WordCount = count;}

    void                                set_json_id(int id) { m_json_id = id; }
    int                                 get_json_id() { return m_json_id; }

};

#endif // CWORD_H
