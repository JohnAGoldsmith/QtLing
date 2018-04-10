#ifndef CSIGNATURE_H
#define CSIGNATURE_H

#include "Stem.h"
#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
#include "Typedefs.h"

class CSuffix;
class CPrefix;
class CStem;
class CSignatureCollection;

sigstring_t restructure_signature(sigstring_t, QString morph, QStringList new_affixes);

class CSignature
{
protected:
    QString                         m_Signature;
    QList<CSuffix*> *               m_Suffixes;
    QList<CPrefix*> *               m_Prefixes;
    QList<CStem*>   *               m_Stems;
    QStringList                     m_Autobiography;
    bool                            m_SuffixFlag;
    CSignatureCollection*           m_SignatureCollection;
    double                          m_stem_entropy;

public:

    CSignature(CSignature&);
    CSignature( QString, bool suffix_flag = true, CSignatureCollection* m_SignatureCollection = NULL);
    ~CSignature();
public:

    // functions dealing with string containment:
    bool                        contains_affix_string (affix_t affix);
    bool                        contains (CSignature* other);
    void                        intersection_with(affix_list& in, affix_list& intersection);


    void                        add_stem (QString);
    void                        add_stem_pointer(CStem*);
    void                        add_memo (QString memo)                 { m_Autobiography.append(memo);}
    void                        add_affix_ptr (CSuffix* pSuffix);       // { m_Suffixes->append(pSuffix);  }
    void                        add_affix_ptr (CPrefix* pPrefix);       // { m_Prefixes->append(pPrefix);  }
    void                        add_affix_string (QString);
    double                      calculate_stem_entropy();
    QString                     display();
    QString                     display_stems() ;
    void                        dump_string_set_of_suffixes(QSet<QString> & this_StringSet);
    int                         get_number_of_stems() const             { return m_Stems->count();}
    int                         get_number_of_affixes() const ;
    CSignatureCollection*       get_signatures()            const       {return m_SignatureCollection;}
    QString                     GetSignature()              const       { return m_Signature; }
    QString                     get_key()                   const       { return m_Signature;}
    QList<CPrefix*>*            get_prefix_list()                       {return m_Prefixes;}
    int                         get_robustness() const;
    double                      get_stem_entropy();
    QList<CStem*>*              get_stems()                             { return   m_Stems;}
    QStringList&                get_stem_strings(QStringList&);
    QStringList&                get_string_list(QStringList&)  ;
    bool                        get_suffix_flag()                       { return m_SuffixFlag;}
    void                        set_suffix_flag(bool flag)              { m_SuffixFlag = flag;}
    QList<CSuffix*>*            get_suffix_list()                       {return m_Suffixes;}
    word_and_count_list *       get_word_and_count_vectors(word_and_count_list* );
    void                        remove_suffix(suffix_t);
    void                        remove_prefix(prefix_t);
    void                        sort_stems();
    void                        sort_stems_by_count();


};

#endif // CSIGNATURE_H
