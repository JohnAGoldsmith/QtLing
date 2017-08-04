#ifndef CSIGNATURE_H
#define CSIGNATURE_H

#include "Parse.h"
#include "StringSurrogate.h"
#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
#include "generaldefinitions.h"

class CSuffix;

class CSignature : public CParse
{
protected:
    QString m_Signature;
    QList<CSuffix*> m_Affixes;

public:
    CSignature(CStringSurrogate ssWord);
    CSignature(CSignature&);

public:
    //Accessors
    QString        GetSignature()               const { return m_Signature; }
    QList<CSuffix*> GetList()                          { return m_Affixes;   }

    QList<CSuffix*>::iterator GetBegin()   { return m_Affixes.begin();  }
    QList<CSuffix*>::iterator GetEnd()     { return m_Affixes.end();    }

};

#endif // CSIGNATURE_H
