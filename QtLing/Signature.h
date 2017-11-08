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


class CSignature
{
protected:
    QString               m_Signature;
    QList<CSuffix*> *     m_Suffixes;
    QList<CPrefix*> *     m_Prefixes;
    QList<CStem*>   *     m_Stems;
    QStringList           m_Autobiography;
    bool                  m_SuffixFlag;

public:

    CSignature(CSignature&);
    CSignature( QString);
    ~CSignature();
public:
    //Accessors
    QString                     GetSignature()              const   { return m_Signature; }
    QString                     get_key()                   const   { return m_Signature;}
    //QList<CSuffix*>*            GetList() ;                      //    { return m_Affixes;   }
    QList<CSuffix*>*            get_suffix_list()                         {return m_Suffixes;}
    QList<CPrefix*>*            get_prefix_list()                         {return m_Prefixes;}
    void                        dump_string_set_of_suffixes(QSet<QString> & this_StringSet);
    void                        add_stem (QString);
    void                        add_stem (CStem* pStem)             { m_Stems->append(pStem);}
    void                        add_stem_pointer(CStem*);
    CSuffix_ptr_list  *         get_CSuffixes ()              const { return m_Suffixes;}
    QList<CStem*>*              get_stems()                         { return   m_Stems;}
    void                        add_memo (QString memo)             { m_Autobiography.append(memo);}
    void                        add_affix_ptr (CSuffix* pSuffix)   { m_Suffixes->append(pSuffix);  }
    void                        add_affix_ptr (CPrefix* pPrefix)   { m_Prefixes->append(pPrefix);  }
    QString                     display();
    QString                     display_stems() ;
    int                         get_number_of_stems() const {return m_Stems->count();}
    int                         get_number_of_affixes() const ;
    int                         get_robustness();
    bool                        get_suffix_flag()               {return m_SuffixFlag;}
    bool                        contains (CSignature* other);
};

#endif // CSIGNATURE_H
