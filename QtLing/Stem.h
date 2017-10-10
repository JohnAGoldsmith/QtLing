#ifndef CSTEM_H
#define CSTEM_H

#include "Parse.h"
#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
#include "generaldefinitions.h"

class CSignature;

class CStem
{
protected:
    QString m_key;
    QList<QString> m_Signatures;
public:
    CStem (QString);
    CStem(CStem&);
public:
    //Accessors
    QString            get_key()       const{return m_key;}
    QString            get_stem()       const { return m_key; }
    QList<QString>*    GetSignatures()  { return &m_Signatures; }

    void                add_signature(QString sig_string ) {m_Signatures.append(sig_string); }

    QList<QString>::iterator GetBegin()   { return m_Signatures.begin();  }
    QList<QString>::iterator GetEnd()     { return m_Signatures.end();  }
    
    QString display();
};

#endif // CSTEM_H
