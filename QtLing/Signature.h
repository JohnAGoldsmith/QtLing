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
class CStem;


class CSignature
{
protected:
    QString               m_Signature;
    QList<CSuffix*> *     m_Affixes;
    QList<CStem*>   *     m_Stems;

public:
   // CSignature(CStringSurrogate ssWord);
    CSignature(CSignature&);
    CSignature( QString);
    ~CSignature();
public:
    //Accessors
    QString                     GetSignature()              const   { return m_Signature; }
    QString                     get_key()                   const   { return m_Signature;}
    QList<CSuffix*>*            GetList()                           { return m_Affixes;   }
    void                        dump_string_set_of_suffixes(QSet<QString> & this_StringSet);
    void                        add_stem (QString);
    void                        add_stem (CStem* pStem)             { m_Stems->append(pStem);}
    void                        add_stem_pointer(CStem*);
    CSuffix_ptr_list  *         get_CSuffixes ()              const { return m_Affixes;}
    QList<CStem*>*              get_stems()                         { return   m_Stems;}
    

    
    QString                     display();
    QString                     display_stems() ;
    int                         get_number_of_stems() const {return m_Stems->count();}
    int                         get_number_of_affixes() const ;
    int                         get_robustness();

};

#endif // CSIGNATURE_H
