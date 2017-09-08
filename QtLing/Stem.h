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
    QString m_key;
    QString m_Word;
    QList<CSignature*> m_Signatures;
public:
    CStem(CStringSurrogate ssWord);
    CStem (QString);
    CStem(CStem&);
public:
    //Accessors
    QString            get_key() const{return m_key;}
    QString            GetStem()       const { return m_Word; }
    QList<CSignature*> GetSignatures() const { return m_Signatures; }

    QList<CSignature*>::iterator GetBegin()   { return m_Signatures.begin();  }
    QList<CSignature*>::iterator GetEnd()     { return m_Signatures.end();  }
    
    QString display();
};

#endif // CSTEM_H
