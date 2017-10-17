#ifndef CSIGNATURE_H
#define CSIGNATURE_H

//#include "Parse.h"
//#include "StringSurrogate.h"
#include "Stem.h"
#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
#include "generaldefinitions.h"

class CSuffix;
class CStem;


class CSignature
{
protected:
    QString             m_Signature;
    QList<CSuffix*>     m_Affixes;
    QList<CStem*>       m_Stems;

public:
   // CSignature(CStringSurrogate ssWord);
    CSignature(CSignature&);
    CSignature( QString);
public:
    //Accessors
    QString        GetSignature()               const { return m_Signature; }
    QString         get_key()                   const {return m_Signature;}
    QList<CSuffix*> GetList()                          { return m_Affixes;   }

    QList<CSuffix*>::iterator GetBegin()   { return m_Affixes.begin();  }
    QList<CSuffix*>::iterator GetEnd()     { return m_Affixes.end();    }
    QList<CStem*>*  get_stems() { return  & m_Stems;}
    
    void add_stem_pointer(CStem*);
    
    QString display();
    QString display_stems() ;
    int get_number_of_stems() const {return m_Stems.count();}
    int get_robustness();

};

#endif // CSIGNATURE_H
