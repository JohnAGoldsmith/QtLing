#ifndef CSTEM_H
#define CSTEM_H

#include "Parse.h"
#include "StringSurrogate.h"
#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
#include "generaldefinitions.h"

class CSignature;

class CStem : public CParse
{
protected:
    QString m_Word;
    QList<CSignature*> m_Signatures;
public:
    CStem(CStringSurrogate ssWord);
    CStem(CStem&);
public:
    //Accessors
    QString            GetWord()       const { return m_Word; }
    QList<CSignature*> GetSignatures() const { return m_Signatures; }

    QList<CSignature*>::iterator GetBegin()   { return m_Signatures.begin();  }
    QList<CSignature*>::iterator GetEnd()     { return m_Signatures.end();  }
};

#endif // CSTEM_H
