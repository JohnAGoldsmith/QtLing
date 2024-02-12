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
#include "spine.h"
#include "morphemeparse.h"

class LowerTableView;

typedef  QPair<CStem*,CSignature*>* ptr_to_stem_sig_pair;

struct Parse_triple {
    QString         m_stem;
    QString         m_suffix;
    QString         m_sig_string;

    Parse_triple(QString stem, QString suffix, QString sig_string)
        {m_stem = stem; m_suffix = suffix;  m_sig_string =sig_string;}
    QString         display();
};


class CWord
{
    friend class LowerTableView;
protected:
    QString                             m_Word;
    int                                 m_WordCount;
    QList<Parse_triple*>                m_suffixal_parse_triple_list;
    QList<Parse_triple*>                m_prefixal_parse_triple_list;
    Spine                               m_suffixal_spine;
    Spine                               m_prefixal_spine;
    MorphemeParse                       m_morpheme_parse; //this replaces m_morphemic_splits below
    QStringList                         m_Morphemic_splits; // a morphemic split is a QString like: "pre bind ing s"
    QList<ptr_to_stem_sig_pair>         m_Signatures;
    int                                 m_json_id;
    QStringList                         m_Compounds;

    // we want to allow a parse of a word to have more than two pieces, like "govern ment s". This is not entirely consistent with the philosphy behind the Parse_triple, which
    // assumed all words had at most a stem and an affix.



public:
    CWord(QString  Word);
    CWord(CWord&);
    ~CWord();
public:
    //Accessors
    void                 add_compound(QString string);
    void                 add_parse(CStem* stem, CSuffix* suffix, CSignature* signature);
    void                 add_suffixal_parse_triple(QString stem, QString affix, sigstring_t this_sig_string);
    void                 add_suffixal_parse_triple(Parse_triple * parse_triple) {m_suffixal_parse_triple_list.append(parse_triple);}
    void                 add_prefixal_parse_triple(QString stem, QString affix, sigstring_t this_sig_string);
    void                 add_parse_triple(QString stem, QString affix, sigstring_t this_sig_string, bool suffix_flag);
    void                 add_morphemic_split(QString);
    QString              display();
    QStringList&         get_morpheme_splits() {return m_Morphemic_splits;}
    void                 remove_morphemic_split(QString morpheme_split);
    void                 add_to_autobiography(QString line);
    //void                                clear_signatures() { m_Signatures.clear();}
    void                 clear_parse_triple_map();
    QString              get_key() {return m_Word;}
    int                  get_word_count() const  { return m_WordCount; }
    QList<Parse_triple*>* get_parse_triple_list()     { return & m_suffixal_parse_triple_list; }
    QList<Parse_triple*>* get_prefixal_parse_triple_list()     { return & m_prefixal_parse_triple_list; }
    bool                 contains_this_stem_among_parses (QString stem);
    QStringList          get_compounds(){return m_Compounds;}
    MorphemeParse *      get_morpheme_parse() {return & m_morpheme_parse;}
    Spine*               get_suffixal_spine();
    Spine*               get_prefixal_spine();
    QString              get_word()      const { return m_Word; }
    QList<ptr_to_stem_sig_pair> get_signatures() {return m_Signatures;}
    void                 IncrementWordCount(int n = 1);
    void                 SetWordCount(int count) { m_WordCount = count;}
    void                 sort_suffixal_parse_triple_list();
    void                 sort_prefixal_parse_triple_list();
    void                 set_json_id(const int id) {m_json_id = id;}
    int                  get_json_id() const {return m_json_id;}
    void                 write_json(QJsonObject& ref_json, eJsonType json_type = INDEXED) const;

};

#endif // CWORD_H
