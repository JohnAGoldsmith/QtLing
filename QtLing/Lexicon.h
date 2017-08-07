#ifndef CLEXICON_H
#define CLEXICON_H

#include <QMap>
#include <QString>


#include "SignatureCollection.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "WordCollection.h"
#include "StringSurrogate.h"

class CLexicon
{
protected:
//    CWordCollection m_Words;
    CStemCollection m_Stems;
    CSuffixCollection m_Suffixes;
//    CSignatureCollection m_Signatures;
public:
    CLexicon();
public:
    // accessors
    QMap<CStringSurrogate, int> m_Protostems;
    CWordCollection m_Words;
    CSignatureCollection m_Signatures;

    void FindProtostems();
    void CreateStemAffixPairs();
    void AssignSuffixesToStems();

    CWordCollection      GetWordCollection() { return m_Words; }
    CSignatureCollection GetSignatures()     { return m_Signatures; }

};

#endif // CLEXICON_H
