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
    //Accessors
    void                        add_stem (QString);
    void                        add_stem_pointer(CStem*);
    void                        add_memo (QString memo)                 { m_Autobiography.append(memo);}
    void                        add_affix_ptr (CSuffix* pSuffix)        { m_Suffixes->append(pSuffix);  }
    void                        add_affix_ptr (CPrefix* pPrefix)        { m_Prefixes->append(pPrefix);  }
    double                      calculate_stem_entropy();
    bool                        contains_affix_string (affix_t affix);
    bool                        contains (CSignature* other);

    QString                     display();
    QString                     display_stems() ;
    void                        dump_string_set_of_suffixes(QSet<QString> & this_StringSet);
    CSignatureCollection*       get_signatures()            const       {return m_SignatureCollection;}
    QString                     GetSignature()              const       { return m_Signature; }
    QString                     get_key()                   const       { return m_Signature;}
    QList<CSuffix*>*            get_suffix_list()                       {return m_Suffixes;}
    QList<CPrefix*>*            get_prefix_list()                       {return m_Prefixes;}
    QStringList                 get_string_list(QStringList&)  ;
    QList<CStem*>*              get_stems()                             { return   m_Stems;}
    int                         get_number_of_stems() const             { return m_Stems->count();}
    int                         get_number_of_affixes() const ;
    int                         get_robustness() const;
    bool                        get_suffix_flag()                       { return m_SuffixFlag;}
    word_and_count_list *       get_word_and_count_vectors(word_and_count_list* );
    double                      get_stem_entropy();

    void                        set_suffix_flag(bool flag)              { m_SuffixFlag = flag;}
    void                        sort_stems();


};

#endif // CSIGNATURE_H
