#ifndef CSIGNATURE_H
#define CSIGNATURE_H

//#include "Parse.h"
//#include "StringSurrogate.h"
#include "Stem.h"
#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
//#include "generaldefinitions.h"
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
    CSignatureCollection*       get_signatures()            const  {return m_SignatureCollection;}
    QString                     GetSignature()              const   { return m_Signature; }
    QString                     get_key()                   const   { return m_Signature;}
//    CSuffix_ptr_list  *         get_CSuffixes ()              const { return m_Suffixes;}
    QList<CSuffix*>*            get_suffix_list()                         {return m_Suffixes;}
    QList<CPrefix*>*            get_prefix_list()                         {return m_Prefixes;}
    QStringList                 get_string_list(QStringList&)  ;
    void                        dump_string_set_of_suffixes(QSet<QString> & this_StringSet);
    void                        add_stem (QString);
//    void                        add_stem (CStem* pStem);
    void                        add_stem_pointer(CStem*);

    QList<CStem*>*              get_stems()                         { return   m_Stems;}
    void                        add_memo (QString memo)             { m_Autobiography.append(memo);}
    void                        add_affix_ptr (CSuffix* pSuffix)   { m_Suffixes->append(pSuffix);  }
    void                        add_affix_ptr (CPrefix* pPrefix)   { m_Prefixes->append(pPrefix);  }
    QString                     display();
    QString                     display_stems() ;
    int                         get_number_of_stems() const {return m_Stems->count();}
    int                         get_number_of_affixes() const ;
    int                         get_robustness() const;
    bool                        get_suffix_flag()               {return m_SuffixFlag;}
    void                        set_suffix_flag(bool flag)      {m_SuffixFlag = flag;}
    bool                        contains (CSignature* other);
    void                        sort_stems();
    word_and_count_list *       get_word_and_count_vectors(word_and_count_list* );
    double                      get_stem_entropy()              {return m_stem_entropy;};
    double                      calculate_stem_entropy();
 ;
};

#endif // CSIGNATURE_H
